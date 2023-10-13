#ifndef _TOKENIZER_
#define _TOKENIZER_

/* ----- Enum Definition----- */
typedef enum {
  TK_RESERVED, 
  TK_NUM,
  TK_EOF,
} TokenKind;

/* ----- Struct Definition----- */
typedef struct Token Token;
struct Token {
  TokenKind kind;
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
  Token *next;
};

/* ----- Gloval Variable ----- */
extern Token* token;
extern char* user_input;

Token* tokenize(char *p);
void error_at(char *loc, char *fmt, ...);

#endif