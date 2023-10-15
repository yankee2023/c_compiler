#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "assembler/code_generator.h"
#include "logger/log.h"
#include "ast/parser.h"

static FILE* s_fp;

static void parse_node(Node* node) {
    if (ND_NUM == node->kind) {
        fprintf(s_fp, "  push %d\n", node->value);
        return;
    }

    parse_node(node->lhs);
    parse_node(node->rhs);

    fprintf(s_fp, "    pop rdi\n");
    fprintf(s_fp, "  pop rax\n");

    switch (node->kind) {
    case ND_ADD:
        fprintf(s_fp, "  add rax, rdi\n");
        break;
    case ND_SUB:
        fprintf(s_fp, "  sub rax, rdi\n");
        break;
    case ND_MUL:
        fprintf(s_fp, "  imul rax, rdi\n");
        break;
    case ND_DIV:
        fprintf(s_fp, "  cqo\n");
        fprintf(s_fp, "  idiv rdi\n");
        break;
    case ND_EQ:
        fprintf(s_fp, "  cmp rax, rdi\n");
        fprintf(s_fp, "  sete al\n");
        fprintf(s_fp, "  movzb rax, al\n");
        break;
    case ND_NE:
        fprintf(s_fp, "  cmp rax, rdi\n");
        fprintf(s_fp, "  setne al\n");
        fprintf(s_fp, "  movzb rax, al\n");
        break;
    case ND_LT:
        fprintf(s_fp, "  cmp rax, rdi\n");
        fprintf(s_fp, "  setl al\n");
        fprintf(s_fp, "  movzb rax, al\n");
        break;
    case ND_LE:
        fprintf(s_fp, "  cmp rax, rdi\n");
        fprintf(s_fp, "  setle al\n");
        fprintf(s_fp, "  movzb rax, al\n");
        break;
    default:
        break;
    }

    fprintf(s_fp, "  push rax\n");
}

void code_gen(Node* node) {
    s_fp = fopen("9cc.s", "w");
    if (NULL == s_fp) {
        log_error("Failed to open file. File pointer is null.");
        exit(1);
    }
    

    // アセンブリの前半部分を出力
    fprintf(s_fp, ".intel_syntax noprefix\n");
    fprintf(s_fp, ".globl main\n");
    fprintf(s_fp, "main:\n");

    parse_node(node);

    // スタックトップに式全体の値が残っているはずなので
    // それをRAXにロードして関数からの返り値とする
    fprintf(s_fp, "    pop rax\n");
    fprintf(s_fp, "    ret\n");
    fprintf(s_fp, "\n");

    fclose(s_fp);
}