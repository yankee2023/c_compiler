#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "assembler/code_generator.h"
#include "logger/log.h"
#include "ast/parser.h"
#include "tokenizer/tokenizer.h"

static FILE* s_fp;

static void gen_lval(Node *node) {
    if (node->kind != ND_LVAR) {
        log_error("代入の左辺値が変数ではありません");
        exit(1);
    }

    fprintf(s_fp, "  mov rax, rbp\n");
    fprintf(s_fp, "  sub rax, %d\n", node->offset);
    fprintf(s_fp, "  push rax\n");
}

static void gen(Node* node) {
    if (ND_NUM == node->kind) {
        fprintf(s_fp, "  push %d\n", node->value);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

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
    case ND_LVAR:
        gen_lval(node);
        printf("  pop rax\n");
        printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        break;
    case ND_ASSIGN:
        gen_lval(node->lhs);
        gen(node->rhs);
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

    // プロローグ
    // 変数26個分の領域を確保する
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n");

    // 先頭の式から順にコード生成
    for (int i = 0; g_code[i]; i++) {
        gen(g_code[i]);

        // 式の評価結果としてスタックに一つの値が残っている
        // はずなので、スタックが溢れないようにポップしておく
        fprintf(s_fp, "  pop rax\n");
    }

    // エピローグ
    // 最後の式の結果がRAXに残っているのでそれが返り値になる
    fprintf(s_fp, "  mov rsp, rbp\n");
    fprintf(s_fp, "  pop rbp\n");
    fprintf(s_fp, "  ret\n");
    fprintf(s_fp, "\n");

    fclose(s_fp);
}