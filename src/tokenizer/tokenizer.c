#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer/tokenizer.h"

/* ----- Gloval Variable -----*/
// 現在着目しているトークン
Token *token;

/**
 * @brief エラーを報告するための関数
 * @param fmt printfと同じ引数
 */ 
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
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

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " "); // pos個の空白を出力
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

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
int expect_number() {
    if (token->kind != TK_NUM) {
        error_at(token->str, "数ではありません");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

/**
 * @brief 新しいトークンを作成してcurに繋げる
*/
Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

/**
 * @brief 入力文字列pをトークナイズしてそれを返す
*/
Token *tokenize(char *p) {
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