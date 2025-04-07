#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Token types
typedef enum {
    TOKEN_NUMBER,   // e.g., 2, 3, 4
    TOKEN_PLUS,     // +
    TOKEN_TIMES,    // *
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_EOF       // End of input
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    int value;      // Holds the number's value for TOKEN_NUMBER
} Token;

// Maximum number of tokens (for simplicity)
#define MAX_TOKENS 100
Token tokens[MAX_TOKENS];
int token_count = 0;

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

// Function prototypes
void tokenize(const char* input);
ASTNode* parse_expression(int* index);
ASTNode* parse_term(int* index);
ASTNode* parse_factor(int* index);
int evaluate(ASTNode* node);
void free_ast(ASTNode* node);

// Tokenize the input string into an array of tokens
void tokenize(const char* input) {
    int pos = 0;
    token_count = 0; // Reset token count
    while (input[pos] != '\0' && token_count < MAX_TOKENS) {
        while (isspace(input[pos])) pos++; // Skip whitespace
        if (input[pos] == '\0') break;
        if (isdigit(input[pos])) {
            int value = 0;
            while (isdigit(input[pos])) {
                value = value * 10 + (input[pos] - '0');
                pos++;
            }
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].value = value;
            token_count++;
        } else if (input[pos] == '+') {
            tokens[token_count].type = TOKEN_PLUS;
            token_count++;
            pos++;
        } else if (input[pos] == '*') {
            tokens[token_count].type = TOKEN_TIMES;
            token_count++;
            pos++;
        } else if (input[pos] == '(') {
            tokens[token_count].type = TOKEN_LPAREN;
            token_count++;
            pos++;
        } else if (input[pos] == ')') {
            tokens[token_count].type = TOKEN_RPAREN;
            token_count++;
            pos++;
        } else {
            printf("Invalid character: %c\n", input[pos]);
            exit(1);
        }
    }
    tokens[token_count].type = TOKEN_EOF;
    token_count++;
}

// Parse factor: number or parenthesized expression
ASTNode* parse_factor(int* index) {
    if (tokens[*index].type == TOKEN_NUMBER) {
        ASTNode* node = malloc(sizeof(ASTNode));
        node->type = NODE_NUMBER;
        node->data.value = tokens[*index].value;
        (*index)++;
        return node;
    } else if (tokens[*index].type == TOKEN_LPAREN) {
        (*index)++; // Consume '('
        ASTNode* expr = parse_expression(index);
        if (tokens[*index].type != TOKEN_RPAREN) {
            printf("Expected ')'\n");
            exit(1);
        }
        (*index)++; // Consume ')'
        return expr;
    } else {
        printf("Expected number or '('\n");
        exit(1);
    }
}

// Parse term: factors separated by '*'
ASTNode*	parse_term(int* index) {
	ASTNode* left = parse_factor(index);
	while (tokens[*index].type == TOKEN_TIMES) {
		TokenType op = tokens[*index].type;
		(*index)++;
		ASTNode* right = parse_factor(index);
		ASTNode* new_node = malloc(sizeof(ASTNode));
		new_node->type = NODE_BINARY_OP;
		new_node->data.binary_op.op = op;
		new_node->data.binary_op.left = left;
		new_node->data.binary_op.right = right;
		left = new_node;
	}
	return left;
}

// Parse expression: terms separated by '+'
ASTNode*    parse_expression(int* index)
{
    ASTNode* left = parse_term(index);
    while (tokens[*index].type == TOKEN_PLUS)
    {
        TokenType op = tokens[*index].type;
        (*index)++;
        ASTNode* right = parse_term(index);
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
int evaluate(ASTNode* node) {
    if (node->type == NODE_NUMBER) {
        return node->data.value;
    } else if (node->type == NODE_BINARY_OP) {
        int left_val = evaluate(node->data.binary_op.left);
        int right_val = evaluate(node->data.binary_op.right);
        if (node->data.binary_op.op == TOKEN_PLUS) {
            return left_val + right_val;
        } else if (node->data.binary_op.op == TOKEN_TIMES) {
            return left_val * right_val;
        } else {
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

const char* token_type_to_str(TokenType op) {
    switch(op) {
        case TOKEN_PLUS: return "+";
        // case TOKEN_: return "-";
        case TOKEN_TIMES: return "*";
        // case TOKEN_DIV: return "/";
        default: return "?";
    }
}

void print_astree(ASTNode* node, int level) {
    if (!node) return;

    // Indentation based on depth
    for (int i = 0; i < level; i++) 
        printf("  >");

    if (node->type == NODE_NUMBER) {
        printf("Number: %d\n", node->data.value);
    } 
    else if (node->type == NODE_BINARY_OP) {
        printf("Operator: %s\n", token_type_to_str(node->data.binary_op.op));
        print_astree(node->data.binary_op.left, level + 1);
        print_astree(node->data.binary_op.right, level + 1);
    }
}


int main() {
    const char* input = "2 - 3 * 4"; // Example input
    // Step 1: Tokenize
    tokenize(input);
    printf("Tokenization complete. Tokens:\n");
    for (int i = 0; i < token_count; i++) {
        switch (tokens[i].type) {
            case TOKEN_NUMBER: printf("NUMBER(%d) ", tokens[i].value); break;
            case TOKEN_PLUS: printf("PLUS "); break;
            case TOKEN_TIMES: printf("TIMES "); break;
            case TOKEN_LPAREN: printf("LPAREN "); break;
            case TOKEN_RPAREN: printf("RPAREN "); break;
            case TOKEN_EOF: printf("EOF\n"); break;
        }
    }

    // Step 2: Parse to build AST
    int index = 0;
	ASTNode* ast = parse_expression(&index);
    if (tokens[index].type != TOKEN_EOF) {
        printf("Unexpected tokens at the end\n");
        exit(1);
    }
    printf("Parsing complete. AST built.\n");
	print_astree(ast, 0);
    // Step 3: Calculate the result
    int result = evaluate(ast);
    printf("Calculation complete. Result: %d\n", result);

    // Clean up
    free_ast(ast);
    return 0;
}