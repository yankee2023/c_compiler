#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer/tokenizer.h"
#include "parser/parser.h"

char *user_input;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    user_input = argv[1];

    token = tokenize(argv[1]);

    parse();

    return 0;
}