#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TOKENS 100

// Token types
typedef enum {
    TOKEN_NUMBER,   // e.g., 2, 3, 4
    TOKEN_PLUS,     // +
    TOKEN_MINUS,    // -
    TOKEN_TIMES,    // *
    TOKEN_DIVIDE,   // /
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_EOF       // End of input
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    int value;      // Holds the number's value for TOKEN_NUMBER
} Token;

// AST node types
typedef enum {
    NODE_NUMBER,
    NODE_BINARY_OP
} NodeType;

// AST node structure
typedef struct ASTNode {
    NodeType type;
    union {
        int value; // For NODE_NUMBER
        struct {
            TokenType op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binary_op;
    } data;
} ASTNode;

// Parser structure to hold tokens and current position
typedef struct {
    Token* tokens;
    int current_index;
    int token_count;
} Parser;

// Function prototypes
void tokenize(const char* input, Token* tokens, int* token_count);
ASTNode* parse_expression(Parser* parser);
ASTNode* parse_term(Parser* parser);
ASTNode* parse_factor(Parser* parser);
int evaluate(ASTNode* node);
void free_ast(ASTNode* node);

// Tokenize the input string into an array of tokens
void tokenize(const char* input, Token* tokens, int* token_count) {
    int pos = 0;
    *token_count = 0;
    while (input[pos] != '\0' && *token_count < MAX_TOKENS) {
        while (isspace(input[pos])) pos++; // Skip whitespace
        if (input[pos] == '\0') break;
        if (isdigit(input[pos])) {
            int value = 0;
            while (isdigit(input[pos])) {
                value = value * 10 + (input[pos] - '0');
                pos++;
            }
            tokens[*token_count].type = TOKEN_NUMBER;
            tokens[*token_count].value = value;
            (*token_count)++;
        } else if (input[pos] == '+') {
            tokens[*token_count].type = TOKEN_PLUS;
            (*token_count)++;
            pos++;
        } else if (input[pos] == '-') {
            tokens[*token_count].type = TOKEN_MINUS;
            (*token_count)++;
            pos++;
        } else if (input[pos] == '*') {
            tokens[*token_count].type = TOKEN_TIMES;
            (*token_count)++;
            pos++;
        } else if (input[pos] == '/') {
            tokens[*token_count].type = TOKEN_DIVIDE;
            (*token_count)++;
            pos++;
        } else if (input[pos] == '(') {
            tokens[*token_count].type = TOKEN_LPAREN;
            (*token_count)++;
            pos++;
        } else if (input[pos] == ')') {
            tokens[*token_count].type = TOKEN_RPAREN;
            (*token_count)++;
            pos++;
        } else {
            printf("Invalid character: %c\n", input[pos]);
            exit(1);
        }
    }
    tokens[*token_count].type = TOKEN_EOF;
    (*token_count)++;
}

// Parse factor: number or parenthesized expression
ASTNode* parse_factor(Parser* parser) {
    if (parser->tokens[parser->current_index].type == TOKEN_NUMBER) {
        ASTNode* node = malloc(sizeof(ASTNode));
        node->type = NODE_NUMBER;
        node->data.value = parser->tokens[parser->current_index].value;
        parser->current_index++;
        return node;
    } else if (parser->tokens[parser->current_index].type == TOKEN_LPAREN) {
        parser->current_index++; // Consume '('
        ASTNode* expr = parse_expression(parser);
        if (parser->tokens[parser->current_index].type != TOKEN_RPAREN) {
            printf("Expected ')'\n");
            exit(1);
        }
        parser->current_index++; // Consume ')'
        return expr;
    } else {
        printf("Expected number or '('\n");
        exit(1);
    }
}

// Parse term: factors separated by '*' or '/'
ASTNode* parse_term(Parser* parser) {
    ASTNode* left = parse_factor(parser);
    while (parser->tokens[parser->current_index].type == TOKEN_TIMES ||
           parser->tokens[parser->current_index].type == TOKEN_DIVIDE) {
        TokenType op = parser->tokens[parser->current_index].type;
        parser->current_index++;
        ASTNode* right = parse_factor(parser);
        ASTNode* new_node = malloc(sizeof(ASTNode));
        new_node->type = NODE_BINARY_OP;
        new_node->data.binary_op.op = op;
        new_node->data.binary_op.left = left;
        new_node->data.binary_op.right = right;
        left = new_node;
    }
    return left;
}

// Parse expression: terms separated by '+' or '-'
ASTNode* parse_expression(Parser* parser) {
    ASTNode* left = parse_term(parser);
    while (parser->tokens[parser->current_index].type == TOKEN_PLUS ||
           parser->tokens[parser->current_index].type == TOKEN_MINUS) {
        TokenType op = parser->tokens[parser->current_index].type;
        parser->current_index++;
        ASTNode* right = parse_term(parser);
        ASTNode* new_node = malloc(sizeof(ASTNode));
        new_node->type = NODE_BINARY_OP;
        new_node->data.binary_op.op = op;
        new_node->data.binary_op.left = left;
        new_node->data.binary_op.right = right;
        left = new_node;
    }
    return left;
}

// Evaluate the AST to compute the result
int evaluate(ASTNode* node) 
{
    if (node->type == NODE_NUMBER) {
        return node->data.value;
    } else if (node->type == NODE_BINARY_OP) {
        int left_val = evaluate(node->data.binary_op.left);
        int right_val = evaluate(node->data.binary_op.right);
        switch (node->data.binary_op.op) {
            case TOKEN_PLUS: return left_val + right_val;
            case TOKEN_MINUS: return left_val - right_val;
            case TOKEN_TIMES: return left_val * right_val;
            case TOKEN_DIVIDE:
                if (right_val == 0) {
                    printf("Division by zero\n");
                    exit(1);
                }
                return left_val / right_val;
            default:
                printf("Unknown operator\n");
                exit(1);
        }
    } else {
        printf("Unknown node type\n");
        exit(1);
    }
}

// Free the AST nodes to prevent memory leaks
void free_ast(ASTNode* node) {
    if (node == NULL) return;
    if (node->type == NODE_BINARY_OP) {
        free_ast(node->data.binary_op.left);
        free_ast(node->data.binary_op.right);
    }
    free(node);
}


// Map TokenType to string
const char* token_type_to_str(TokenType type) {
    switch (type) {
        case TOKEN_PLUS: return "+";
        case TOKEN_MINUS: return "-";
        case TOKEN_TIMES: return "*";
        case TOKEN_DIVIDE: return "/";
        default: return "UNKNOWN";
    }
}

// Print the AST as a tree
void print_ast(ASTNode* node, int depth) {
    if (node == NULL) return;

    // Print indentation
	
    for (int i = 0; i < depth; i++) {
        printf("  |"); // Two spaces per level
    }

    if (node->type == NODE_NUMBER) {
        printf("(%d)\n", node->data.value);
    } else if (node->type == NODE_BINARY_OP) {
        const char* op_str = token_type_to_str(node->data.binary_op.op);
        printf("OPERATOR(%s)\n", op_str);
        print_ast(node->data.binary_op.left, depth + 1);
        print_ast(node->data.binary_op.right, depth + 1);
    } else {
        printf("UNKNOWN NODE TYPE\n");
    }
}

int main() {
    const char* input = "2 + 3 * (4 - 6 / 3) -    3     +  4"; // Example input
    Token tokens[MAX_TOKENS];
    int token_count = 0;

    // Tokenize the input
    tokenize(input, tokens, &token_count);

    // Parse to build AST
    Parser parser = {tokens, 0, token_count};
    ASTNode* ast = parse_expression(&parser);
	print_ast(ast, 0); // Print the AST
    if (parser.tokens[parser.current_index].type != TOKEN_EOF) {
        printf("Unexpected tokens at the end\n");
        exit(1);
    }

    // Calculate the result
    int result = evaluate(ast);
    printf("Result: %d\n", result);

    // Clean up
    free_ast(ast);
    return 0;
}