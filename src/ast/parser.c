#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tokenizer/tokenizer.h"
#include "ast/parser.h"
#include "logger/log.h"

static const char* s_node_kind_str[] = { "ND_ADD", "ND_SUB", "ND_MUL", "ND_DIV", "ND_NUM" };

static Node* expr();
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
 * @brief 次のトークンが期待している記号のときには、トークンを1つ読み進めて真を返す。
 * それ以外の場合には偽を返す。
 * @param [in] op 期待している記号
 * @return true:見つかった false:見つからなかった
*/
bool consume(char op) {
    if (g_token->kind != TK_RESERVED || g_token->str[0] != op) {
        return false;
    }

    log_debug("Consume %c.", g_token->str[0]);
    g_token = g_token->next;
    return true;
}

/**
 * @brief 次のトークンが期待している記号のときには、トークンを1つ読み進める。
 * それ以外の場合にはエラーを報告する。
*/
void expect(char op) {
    if (g_token->kind != TK_RESERVED || g_token->str[0] != op) {
        error_at(g_token->str, "'%c'ではありません", op);
    }

    log_debug("Existed %c.", g_token->str[0]);
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
 * @brief primary = num | "(" expr ")"
 * @brief Node型ポインタ
*/
static Node* primary() {
    // 次のトークンが"("なら、"(" expr ")"のはず
    if (consume('(')) {
        Node* node = expr();
        expect(')');
        return node;
    }

    // そうでなければ数値のはず
    return create_new_node_num(expect_number());    
}

/**
 * @brief unary = ("+" | "-")? primary
 * @details -xを0-xに置き換えて単項マイナスを実現している
 * @return Node型ポインタ
*/
static Node* unary() {
    if (consume('+')) {
        return primary();
    }
    if (consume('-')) {
        return create_new_node(ND_SUB, create_new_node_num(0), primary());
    }

    return primary();
}

/**
 * @brief mul = unary ("*" unary | "/" unary)*
 * @return Node型ポインタ
*/
static Node* mul() {
    Node* node = unary();

    for (;;) {
        if (consume('*')) {
            node = create_new_node(ND_MUL, node, unary());
        } else if (consume('/')) {
            node = create_new_node(ND_DIV, node, unary());
        } else {
            return node;
        }
    }
}

/**
 * @brief expr = mul ("+" mul | "-" mul)*
 * @return Node型ポインタ
*/
static Node* expr() {
    Node* node = mul();

    for (;;) {
        if (consume('+')) {
            node = create_new_node(ND_ADD, node, mul());
        } else if (consume('-')) { 
            node = create_new_node(ND_SUB, node, mul());
        } else {
            return node;
        }
    }
}

/**
 * @brief 
 * @return Node型ポインタ
*/
Node* parse() {
    return expr();
}