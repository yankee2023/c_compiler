#ifndef _TOKENIZER_
#define _TOKENIZER_

/* ----- Enum Definition ----- */
typedef enum {
  TK_RESERVED,  // 記号
  TK_IDENT,     // 識別子
  TK_NUM,       // 整数トークン
  TK_EOF,       // 入力の終わりを表すトークン
} TokenKind;

/* ----- Struct Definition ----- */
typedef struct Token Token;
struct Token {
  TokenKind kind;     // トークンの型
  int32_t   val;      // kindがTK_NUMの場合、その数値
  char      *str;     // トークン文字列
  int32_t   len;      // トークンの長さ
  Token     *next;    // 次の入力トークン
};

/* ----- Gloval Variable ----- */
extern Token* g_token;
extern char* g_user_input;

Token* tokenize(char *p);
void error_at(char *loc, char *fmt, ...);

#endif