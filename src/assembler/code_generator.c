#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "assembler/code_generator.h"
#include "logger/log.h"
#include "ast/parser.h"

static FILE* fp;

static void parse_node(Node* node) {
    if (ND_NUM == node->kind) {
        fprintf(fp, "  push %d\n", node->value);
        return;
    }

    parse_node(node->lhs);
    parse_node(node->rhs);

    fprintf(fp, "    pop rdi\n");
    fprintf(fp, "  pop rax\n");

    switch (node->kind) {
    case ND_ADD:
        fprintf(fp, "  add rax, rdi\n");
        break;
    case ND_SUB:
        fprintf(fp, "  sub rax, rdi\n");
        break;
    case ND_MUL:
        fprintf(fp, "  imul rax, rdi\n");
        break;
    case ND_DIV:
        fprintf(fp, "  cqo\n");
        fprintf(fp, "  idiv rdi\n");
        break;
    default:
        break;
    }

    fprintf(fp, "  push rax\n");
}

void code_gen(Node* node) {
    fp = fopen("9cc.s", "w");
    if (NULL == fp) {
        log_error("Failed to open file. File pointer is null.");
        exit(1);
    }
    

    // アセンブリの前半部分を出力
    fprintf(fp, ".intel_syntax noprefix\n");
    fprintf(fp, ".globl main\n");
    fprintf(fp, "main:\n");

    parse_node(node);

    // スタックトップに式全体の値が残っているはずなので
    // それをRAXにロードして関数からの返り値とする
    fprintf(fp, "    pop rax\n");
    fprintf(fp, "    ret\n");
    fprintf(fp, "\n");

    fclose(fp);
}