#ifndef _PARSER_
#define _PARSER_

#include <stdint.h>

typedef enum
{
    ND_ADD,     // +
    ND_SUB,     // -
    ND_MUL,     // *
    ND_DIV,     // /
    ND_ASSIGN,  // =
    ND_LVAR,    // ローカル変数
    ND_EQ,      // ==
    ND_NE,      // !=
    ND_LT,      // <
    ND_LE,      // <=
    ND_NUM,     // 整数
} NodeKind;

typedef struct Node Node;
struct Node
{
    NodeKind kind;  // ノードの型
    Node *lhs;      // 左辺(left hand side)
    Node *rhs;      // 右辺(right hand side)
    int32_t value;  // kindがND_NUMの場合のみ使う
    /*
    kindがND_LVARの場合のみ使う
    ローカル変数のベースポインタからのオフセットを表す
    */
    int32_t offset; 
};

extern Node* g_code[];

Node* parse();

#endif