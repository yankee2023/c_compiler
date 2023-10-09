#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tokenizer/tokenizer.h"
#include "logger/log.h"

/* ----- Gloval Variable -----*/
// 現在着目しているトークン
Token *token;

/**
 * @brief エラーを報告するための関数
 * @param fmt log_debugと同じ引数
 */ 
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log_error(fmt, ap);
    exit(1);
}

/**
 * @brief エラー箇所を報告する
 * @param loc
 * @param fmt
*/
void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int32_t pos = loc - user_input;
    log_error("%s", user_input);
    log_error("%*s", pos, " "); // pos個の空白を出力
    log_error("^ ");
    log_error(fmt, ap);
    exit(1);
}

/**
 * @brief 新しいトークンを作成してcurに繋げる
*/
Token* new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

/**
 * @brief 入力文字列pをトークナイズしてそれを返す
*/
Token* tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        // 空白文字をスキップ
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-') {
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error("トークナイズできません");
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}