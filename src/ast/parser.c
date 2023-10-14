#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tokenizer/tokenizer.h"
#include "ast/parser.h"
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
        error_at(token->str, "'%c'ではありません", op);
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

    FILE* fp;
    fp = fopen("9cc.s", "w");

    fprintf(fp, ".intel_syntax noprefix\n");
    fprintf(fp, ".globl main\n");
    fprintf(fp, "main:\n");

    // 式の最初は数でなければならないので、それをチェックして
    // 最初のmov命令を出力
    fprintf(fp, "    mov rax, %d\n", expect_number());

    while (!at_eof())
    {
        if(consume('+')) {
            fprintf(fp, "    add rax, %d\n", expect_number());
            continue;
        }

        expect('-');
        fprintf(fp, "    sub rax, %d\n", expect_number());
    }

    fprintf(fp, "    ret\n");
    fprintf(fp, "\n");

    fclose(fp);
}