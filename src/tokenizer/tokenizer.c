#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tokenizer/tokenizer.h"
#include "logger/log.h"

/* ----- Gloval Variable -----*/
// 現在着目しているトークン
Token *g_token;

/**
 * @brief エラー箇所を報告する
 * @param loc エラー箇所のある文字列
 * @param fmt エラーメッセージ
*/
void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char this[8] = "^ ";
    char brank[64] = "";
    char message[256] = "";

    int32_t pos = loc - g_user_input;
    log_error("%s", g_user_input);
    for (size_t i = 0; i < pos; i++) {
        brank[i] = ' ';
    }
    strncat(message, brank, strlen(brank));
    strncat(message, this, strlen(this));
    strncat(message, fmt, strlen(fmt));
    log_error(message);
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

        if ('+' == *p || '-' == *p || '*' == *p || '/' == *p || '(' == *p || ')' == *p) {
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error_at(p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}