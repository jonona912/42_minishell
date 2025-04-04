#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int parse_expression();

typedef enum {
    TOKEN_NUMBER,   // e.g., 2, 3, 4
    TOKEN_PLUS,     // +
    TOKEN_TIMES,    // *
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_EOF       // End of input
} TokenType;

typedef struct {
    TokenType type;
    int value;      // Holds the number’s value for TOKEN_NUMBER
} Token;

char *input;        // Global input string
int pos = 0;        // Current position in input
Token current_token; // Current token being processed

void next_token() {
    while (isspace(input[pos])) pos++; // Skip whitespace
    if (input[pos] == '\0') {
        current_token.type = TOKEN_EOF;
        return;
    }
    if (isdigit(input[pos])) {
        int value = 0;
        while (isdigit(input[pos])){
            value = value * 10 + (input[pos] - '0');
            pos++;
        }
        current_token.type = TOKEN_NUMBER;
        current_token.value = value;
    } else if (input[pos] == '+') {
        current_token.type = TOKEN_PLUS;
        pos++;
    } else if (input[pos] == '*') {
        current_token.type = TOKEN_TIMES;
        pos++;
    } else if (input[pos] == '(') {
        current_token.type = TOKEN_LPAREN;
        pos++;
    } else if (input[pos] == ')') {
        current_token.type = TOKEN_RPAREN;
        pos++;
    } else {
        printf("Invalid character: %c\n", input[pos]);
        exit(1);
    }
}

// functions for each grammar rule, evaluating the expression as we go:
int parse_factor() {
    if (current_token.type == TOKEN_NUMBER) {
        int value = current_token.value;
        next_token();
        return value;
    } else if (current_token.type == TOKEN_LPAREN) {
        next_token(); // Consume '('
        int expr = parse_expression();
        if (current_token.type != TOKEN_RPAREN) {
            printf("Expected ')'\n");
            exit(1);
        }
        next_token(); // Consume ')'
        return expr;
    } else {
        printf("Expected number or '('\n");
        exit(1);
    }
}

int parse_term()
{
    int left = parse_factor();
    while (current_token.type == TOKEN_TIMES) {
        next_token();
        int right = parse_factor();
        left *= right;
    }
    return left;
}

int parse_expression() {
    int left = parse_term();
    while (current_token.type == TOKEN_PLUS) {
        next_token();
        int right = parse_term();
        left += right;
    }
    return left;
}

int main() {
    input = "2      + 3*4"; // Try "(2 + 3) * 4" too!
    pos = 0;
    next_token();        // Get the first token
    int result = parse_expression();
    printf("Result: %d\n", result);
    return 0;
}
