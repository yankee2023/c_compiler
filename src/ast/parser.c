#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tokenizer/tokenizer.h"
#include "ast/parser.h"
#include "logger/log.h"

const char* node_kind_str[] = { "ND_ADD", "ND_SUB", "ND_MUL", "ND_DIV", "ND_NUM" };

static Node* expr();
static Node* mul();
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
    log_debug("Create %s.", node_kind_str[new_node->kind]);
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
    log_debug("Create %s. Value is %d", node_kind_str[new_node->kind], new_node->value);
    return new_node;
}

/**
 * @brief 次のトークンが期待している記号のときには、トークンを1つ読み進めて真を返す。
 * それ以外の場合には偽を返す。
*/
bool consume(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op) {
        return false;
    }

    log_debug("Consume %c.", token->str[0]);
    token = token->next;
    return true;
}

/**
 * @brief 次のトークンが期待している記号のときには、トークンを1つ読み進める。
 * それ以外の場合にはエラーを報告する。
*/
void expect(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op) {
        error_at(token->str, "'%c'ではありません", op);
    }

    log_debug("Existed %c.", token->str[0]);
    token = token->next;
}

/**
 * @brief 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
 * それ以外の場合にはエラーを報告する。
*/
int32_t expect_number() {
    if (token->kind != TK_NUM) {
        error_at(token->str, "数ではありません");
    }
    int32_t val = token->val;

    log_debug("Existed %d.", token->val);
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

/**
 * @brief primary = num | "(" expr ")"
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
 * @brief mul = primary ("*" primary | "/" primary)*
*/
static Node* mul() {
    Node* node = primary();

    for (;;) {
        if (consume('*')) {
            node = create_new_node(ND_MUL, node, primary());
        } else if (consume('/')) {
            node = create_new_node(ND_DIV, node, primary());
        } else {
            return node;
        }
    }
}

/**
 * @brief expr = mul ("+" mul | "-" mul)*
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
*/
Node* parse() {
    return expr();
}