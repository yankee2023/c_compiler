#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tokenizer/tokenizer.h"
#include "parser/parser.h"
#include "logger/log.h"

char *user_input;

int32_t main(int32_t argc, char *argv[]) {
    if (argc != 2) {
        log_error("引数の個数が正しくありません");
        return 1;
    }

    user_input = argv[1];

    token = tokenize(argv[1]);

    parse();

    return 0;
}