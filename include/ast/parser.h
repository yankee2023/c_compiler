#ifndef _PARSER_
#define _PARSER_

typedef enum {
    ND_ADD,     // +
    ND_SUB,     // -
    ND_MUL,     // *
    ND_DIV,     // /
    DN_NUM,     // 整数
} NodeKind;

typedef struct Node Node;
struct Node {
    NodeKind kind;  // ノードの型
    Node* lhs;      // 左辺(left hand side)
    Node* rhs;      // 右辺(right hand side)
    int value;      // kindがND_NUMの場合のみ使う
};

void parse();

#endif