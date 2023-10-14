#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "logger/log.h"
#include "tokenizer/tokenizer.h"
#include "ast/parser.h"
#include "assembler/code_generator.h"

char *g_user_input;

int32_t main(int32_t argc, char *argv[]) {
    #if DEBUG
    log_set_level(LOG_TRACE);
    #else
    log_set_level(LOG_INFO);
    #endif

    if (argc != 2) {
        log_error("引数の個数が正しくありません");
        return 1;
    }

    g_user_input = argv[1];
    g_token = tokenize(g_user_input);

    Node* node = parse();

    code_gen(node);

    return 0;
}