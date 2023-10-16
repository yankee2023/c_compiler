#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tokenizer/tokenizer.h"
#include "ast/parser.h"
#include "logger/log.h"

static const char* s_node_kind_str[] = {"ND_ADD", "ND_SUB", "ND_MUL", "ND_DIV",
                                        "ND_EQ", "ND_NE", "ND_LT", "ND_LE",
                                        "ND_NUM"};

static Node* expr();
static Node* equality();
static Node* relational();
static Node* add();
static Node* mul();
static Node* unary();
static Node* primary();

/**
 * @brief 新規ノード作成
 * @details 関数内でNodeをアロケートしデータ書き込み
 * @param [in] kind ノード種類
 * @param [in] lhs 右辺
 * @param [in] rhs 左辺
 * @return 新規ノード
*/
Node* create_new_node(NodeKind kind, Node* lhs, Node* rhs) {
    Node* new_node = calloc(1, sizeof(Node));
    new_node->kind = kind;
    new_node->lhs = lhs;
    new_node->rhs = rhs;
    log_debug("Create %s.", s_node_kind_str[new_node->kind]);
    return new_node;
}

/**
 * @brief 新規数値ノード作成
 * @param [in] value 数値
 * @return 新規数値ノード
*/
Node* create_new_node_num(int32_t value) {
    Node* new_node = calloc(1, sizeof(Node));
    new_node->kind = ND_NUM;
    new_node->value = value;
    log_debug("Create %s. Value is %d", s_node_kind_str[new_node->kind], new_node->value);
    return new_node;
}

/**
 * @brief 次のトークンが期待している文字列のときには、トークンを1つ読み進めて真を返す。
 * それ以外の場合には偽を返す。
 * @param [in] op 期待している文字列
 * @return true:見つかった false:見つからなかった
*/
bool consume(char* op) {
    if (g_token->kind != TK_RESERVED || strlen(op) != g_token->len || memcmp(g_token->str, op, g_token->len)) {
        return false;
    }

    log_debug("Consume %s.", g_token->str);
    g_token = g_token->next;
    return true;
}

/**
 * @brief 次のトークンが期待している記号のときには、トークンを1つ読み進める。
 * それ以外の場合にはエラーを報告する。
*/
void expect(char* op) {
    if (g_token->kind != TK_RESERVED || strlen(op) != g_token->len || memcmp(g_token->str, op, g_token->len)) {
        error_at(g_token->str, "'%s'ではありません", op);
    }

    log_debug("Existed %s.", g_token->str);
    g_token = g_token->next;
}

/**
 * @brief 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
 * それ以外の場合にはエラーを報告する。
*/
int32_t expect_number() {
    if (g_token->kind != TK_NUM) {
        error_at(g_token->str, "数ではありません");
    }
    int32_t val = g_token->val;

    log_debug("Existed %d.", g_token->val);
    g_token = g_token->next;
    return val;
}

bool at_eof() {
    return g_token->kind == TK_EOF;
}

/**
 * @brief expr = equality
 * @return Node型ポインタ
*/
static Node* expr() {
    return equality();
}

/**
 * @brief equality = relational ("==" relational | "!=" relational)*
*/
static Node* equality() {
    Node* node = relational();

    for (;;) {
        if (consume("==")) {
            node = create_new_node(ND_EQ, node, relational());
        } else if (consume("!=")) {
            node = create_new_node(ND_NE, node, relational());
        } else {
            return node;
        }   
    }
}

/**
 * @brief relational = add ("<" add | "<=" add | ">" add | ">= add")*
*/
static Node* relational() {
    Node* node = add();

    for (;;) {
        if (consume("<")) {
            node = create_new_node(ND_LT, node, add());
        } else if (consume("<=")) {
            node = create_new_node(ND_LE, node, add());
        } else if (consume(">")) {
            node = create_new_node(ND_LT, add(), node);
        } else if (consume(">=")) {
            node = create_new_node(ND_LE, add(), node);
        } else {
            return node;
        }
    }
}

/**
 * @brief add = mul ("+" mul | "-" mul)*
*/
static Node* add() {
    Node* node = mul();

    for (;;) {
        if (consume("+")) {
            node = create_new_node(ND_ADD, node, mul());
        } else if (consume("-")) { 
            node = create_new_node(ND_SUB, node, mul());
        } else {
            return node;
        }
    }
}

/**
 * @brief primary = num | "(" expr ")"
 * @brief Node型ポインタ
*/
static Node* primary() {
    // 次のトークンが"("なら、"(" expr ")"のはず
    if (consume("(")) {
        Node* node = expr();
        expect(")");
        return node;
    }

    // そうでなければ数値のはず
    return create_new_node_num(expect_number());    
}

/**
 * @brief mul = unary ("*" unary | "/" unary)*
 * @return Node型ポインタ
*/
static Node* mul() {
    Node* node = unary();

    for (;;) {
        if (consume("*")) {
            node = create_new_node(ND_MUL, node, unary());
        } else if (consume("/")) {
            node = create_new_node(ND_DIV, node, unary());
        } else {
            return node;
        }
    }
}

/**
 * @brief unary = ("+" | "-")? primary
 * @details -xを0-xに置き換えて単項マイナスを実現している
 * @return Node型ポインタ
*/
static Node* unary() {
    if (consume("+")) {
        return primary();
    }
    if (consume("-")) {
        return create_new_node(ND_SUB, create_new_node_num(0), primary());
    }

    return primary();
}

/**
 * @brief parse = expr
 * @return Node型ポインタ
*/
Node* parse() {
    return expr();
}