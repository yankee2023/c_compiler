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

char* token_kind_dic[] = {"TK_RESERVED", "TK_IDENT", "TK_NUM", "TK_EOF"};

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
 * @brief 文字列比較
 * @return true:同じ, false:違う
*/
bool startswitch(char* p, char* q) {
    return memcmp(p, q, strlen(q)) == 0;
}

/**
 * @brief 現在トークンに文字列のデータセットを行う
 * @param [in] kind 現在トークンにセットしたいトークン種類
 * @param [in] cur 現在トークンのポインタ
 * @param [in] str 現在トークンにセットしたい文字列
 * @param [in] len 現在トークンにセットしたい長さ
 * @return 新規トークン
*/
Token* new_token(TokenKind kind, Token *cur, char *str, int32_t len) {
    cur->kind = kind;
    cur->str = str;
    cur->len = len;
    if (TK_NUM == kind) {
        log_debug("Create %s %d %d.", token_kind_dic[kind], cur->val, len);
    } else {
        log_debug("Create %s %s %d.", token_kind_dic[kind], str, len);
    }

    Token *tok = calloc(1, sizeof(Token));
    cur->next = tok;
    return tok;
}

/**
 * @brief 現在トークンに数値のデータセットを行う
 * @param [in] kind 現在トークンにセットしたいトークン種類
 * @param [in] cur 現在トークンのポインタ
 * @param [in] num 現在トークンにセットしたい数値
 * @param [in] len 現在トークンにセットしたい長さ
 * @return 新規トークン
*/
Token* new_num_token(TokenKind kind, Token* cur, int32_t num, int32_t len) {
    cur->val = num;
    return new_token(kind, cur, NULL, len);
}

/**
 * @brief 入力文字列pをトークナイズしてそれを返す
*/
Token* tokenize() {
    Token head;
    head.next = NULL;
    Token *cur = &head;
    char* p = g_user_input;

    while (*p) {
        // 空白文字をスキップ
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (startswitch(p, "==") || startswitch(p, "!=") || startswitch(p, "<=") || startswitch(p, ">=")) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if ('a' <= *p && *p <= 'z') {
            cur = new_token(TK_IDENT, cur, p, 1);
            cur->len = 1;
            p++;
            continue;
        }

        if (strchr("+-*/()<>;", *p)) {
            cur = new_token(TK_RESERVED, cur, p, 1);
            p++;
            continue;
        }
        
        if (isdigit(*p)) {
            char* q = p;
            int32_t num = (int32_t)strtol(p, &p, 10);
            cur = new_num_token(TK_NUM, cur, num, p-q);            
            continue;
        }

        if ('a' <= *p && *p <= 'z') {
            cur = new_token(TK_IDENT, cur, p, 1);
            p++;
            continue;
        }        

        error_at(p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}