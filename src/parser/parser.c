#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tokenizer/tokenizer.h"
#include "parser/parser.h"
#include "logger/log.h"

/**
 * @brief 次のトークンが期待している記号のときには、トークンを1つ読み進めて真を返す。
 * それ以外の場合には偽を返す。
*/
bool consume(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op) {
        return false;
    }
    token = token->next;
    return true;
}

/**
 * @brief 次のトークンが期待している記号のときには、トークンを1つ読み進める。
 * それ以外の場合にはエラーを報告する。
*/
void expect(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op) {
        error("'%c'ではありません", op);
    }
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
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

void parse() {
    log_debug(".intel_syntax noprefix");
    log_debug(".globl main");
    log_debug("main:");

    // 式の最初は数でなければならないので、それをチェックして
    // 最初のmov命令を出力
    log_debug("    mov rax, %d", expect_number());

    while (!at_eof())
    {
        if(consume('+')) {
            log_debug("    add rax, %d", expect_number());
            continue;
        }

        expect('-');
        log_debug("    sub rax, %d", expect_number());
    }

    log_debug("    ret");
}