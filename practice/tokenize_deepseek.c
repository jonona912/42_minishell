#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_OPERATOR,
    TOKEN_IO_NUMBER,
    TOKEN_WORD,
    TOKEN_END
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char *value;
} Token;

// Simple tokenizer (does NOT handle quotes or complex cases)
Token *tokenize(const char *input) {
    Token *tokens = malloc(100 * sizeof(Token)); // Simplified: fixed-size buffer
    int token_index = 0;
    int pos = 0;

    while (input[pos] != '\0') {
        // Skip whitespace
        if (isspace(input[pos])) {
            pos++;
            continue;
        }

        // Check for operators: <, >, |, etc.
        if (strchr("<>|&;", input[pos])) {
            tokens[token_index].type = TOKEN_OPERATOR;
            tokens[token_index].value = strndup(&input[pos], 1);
            pos++;
            token_index++;
            continue;
        }

        // Check for IO_NUMBER (e.g., "2>file.txt")
        if (isdigit(input[pos])) {
            int start = pos;
            while (isdigit(input[pos])) pos++;
            if (input[pos] == '<' || input[pos] == '>') {
                tokens[token_index].type = TOKEN_IO_NUMBER;
                tokens[token_index].value = strndup(&input[start], pos - start + 1);
                pos++;
                token_index++;
                continue;
            } else {
                pos = start; // Not IO_NUMBER; backtrack
            }
        }

        // Default: treat as WORD
        int start = pos;
        while (!isspace(input[pos]) && !strchr("<>|&;", input[pos]) && input[pos] != '\0') {
            pos++;
        }
        tokens[token_index].type = TOKEN_WORD;
        tokens[token_index].value = strndup(&input[start], pos - start);
        token_index++;
    }

    tokens[token_index].type = TOKEN_END;
    return tokens;
}


int main() {
    const char *input = "sort < test.txt | grep z | wc -l";
    Token *tokens = tokenize(input);

    for (int i = 0; tokens[i].type != TOKEN_END; i++) {
        printf("Token %d: Type=%d, Value='%s'\n", i, tokens[i].type, tokens[i].value);
    }

    return 0;
}
