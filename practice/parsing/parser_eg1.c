#include <stdio.h>
#include <ctype.h>

// Token types
typedef enum { NUMBER, PLUS, MINUS, MULTIPLY, DIVIDE, END } TokenType;

// Token structure
typedef struct {
    TokenType type;
    double value;
} Token;

// const char* input = "3 + 4 * 2"; // Input string
const char* pos = "3+      4*2"; //input;         // Current position

// Lexer: Get next token
Token get_next_token() {
    while (*pos == ' ') pos++; // Skip whitespace

    if (*pos == '\0') return (Token){END, 0};

    if (isdigit(*pos)) {
        double num = 0;
        while (isdigit(*pos)) {
            num = num * 10 + (*pos - '0');
            pos++;
        }
        return (Token){NUMBER, num};
    }

    // Handle operators
    char op = *pos++;
    switch (op) {
        case '+': return (Token){PLUS, 0};
        case '-': return (Token){MINUS, 0};
        case '*': return (Token){MULTIPLY, 0};
        case '/': return (Token){DIVIDE, 0};
        default:  return (Token){END, 0}; // Simplified error
    }
}

// Parser functions
double parse_factor();
double parse_term();
double parse_expression();

double parse_expression() {
    double result = parse_term();
    Token token = get_next_token();
    while (token.type == PLUS || token.type == MINUS) {
        double term = parse_term();
        if (token.type == PLUS) result += term;
        else result -= term;
        token = get_next_token();
    }
    pos--; // "Unget" the last token
    return result;
}

double parse_term() {
    double result = parse_factor();
    Token token = get_next_token();
    while (token.type == MULTIPLY || token.type == DIVIDE) {
        double factor = parse_factor();
        if (token.type == MULTIPLY) result *= factor;
        else result /= factor;
        token = get_next_token();
    }
    pos--; // "Unget" the last token
    return result;
}

double parse_factor() {
    Token token = get_next_token();
    if (token.type == NUMBER) return token.value;
    return 0; // Simplified error
}

int main() {
    double result = parse_expression();
    printf("Result: %.2f\n", result); // Output: Result: 11.00
    return 0;
}
