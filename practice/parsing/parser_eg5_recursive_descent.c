#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

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

// Token structure (linked list)
typedef struct Token {
    TokenType type;
    int value;          // For TOKEN_NUMBER
    struct Token* next;
} Token;

// AST node types
typedef enum {
    
    NODE_NUMBER,
    NODE_BINARY_OP
} NodeType;

// AST node structure
typedef struct ASTNode
{
    NodeType type; // CMD_PIPE_IN / OUT  | Redirection
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
Token* tokenize(const char* input);
Token* parse_expression(Token* token, ASTNode** node);
Token* parse_term(Token* token, ASTNode** node);
Token* parse_factor(Token* token, ASTNode** node);
int evaluate(ASTNode* node);
void free_tokens(Token* token);
void free_ast(ASTNode* node);
void print_ast(ASTNode* node, int depth);

// Tokenize input into a linked list
Token* tokenize(const char* input) {
    Token* head = NULL;
    Token* current = NULL;
    int pos = 0;

    while (input[pos] != '\0') {
        while (isspace(input[pos])) pos++; // Skip whitespace
        if (input[pos] == '\0') break;

        Token* new_token = malloc(sizeof(Token));
        new_token->next = NULL;

        if (isdigit(input[pos])) {
            int value = 0;
            while (isdigit(input[pos])) {
                value = value * 10 + (input[pos] - '0');
                pos++;
            }
            new_token->type = TOKEN_NUMBER;
            new_token->value = value;
        } else if (input[pos] == '+') {
            new_token->type = TOKEN_PLUS;
            pos++;
        } else if (input[pos] == '-') {
            new_token->type = TOKEN_MINUS;
            pos++;
        } else if (input[pos] == '*') {
            new_token->type = TOKEN_TIMES;
            pos++;
        } else if (input[pos] == '/') {
            new_token->type = TOKEN_DIVIDE;
            pos++;
        } else if (input[pos] == '(') {
            new_token->type = TOKEN_LPAREN;
            pos++;
        } else if (input[pos] == ')') {
            new_token->type = TOKEN_RPAREN;
            pos++;
        } else {
            printf("Invalid character: %c\n", input[pos]);
            exit(1);
        }

        if (head == NULL) {
            head = current = new_token;
        } else {
            current->next = new_token;
            current = new_token;
        }
    }

    // Add EOF token
    Token* eof = malloc(sizeof(Token));
    eof->type = TOKEN_EOF;
    eof->next = NULL;
    if (head == NULL) {
        head = eof;
    } else {
        current->next = eof;
    }
    return head;
}

// Parse a factor: number or (expression)
Token* parse_factor(Token* token, ASTNode** node)
{
    if (token->type == TOKEN_NUMBER)
	{
        *node = malloc(sizeof(ASTNode));
        (*node)->type = NODE_NUMBER;
        (*node)->data.value = token->value;
        return token->next;
    } else if (token->type == TOKEN_LPAREN)
	{
        token = token->next; // Skip '('
        token = parse_expression(token, node);
        if (token->type != TOKEN_RPAREN) {
            printf("Expected ')'\n");
            exit(1);
        }
        return token->next; // Skip ')'
    } else
	{
        printf("Expected number or '('\n");
        exit(1);
    }
}

Token* handle_binary_operation(Token* token, ASTNode** node) // 2 + 3 - 4 + 5 + 
{
    TokenType op = token->type;
    token = token->next; // Skip operator
    ASTNode* right;
    token = parse_term(token, &right);
    ASTNode* new_node = malloc(sizeof(ASTNode));
    new_node->type = NODE_BINARY_OP;
    new_node->data.binary_op.op = op;
    new_node->data.binary_op.left = *node;
    new_node->data.binary_op.right = right;
    *node = new_node;
    return token;
}

// Parse a term: factors with * or /
Token* parse_term(Token* token, ASTNode** node) {
    token = parse_factor(token, node);
    while (token->type == TOKEN_TIMES || token->type == TOKEN_DIVIDE) {
        // TokenType op = token->type;
        // token = token->next; // Skip operator
        // ASTNode* right;
        // token = parse_factor(token, &right);
        // ASTNode* new_node = malloc(sizeof(ASTNode));
        // new_node->type = NODE_BINARY_OP;
        // new_node->data.binary_op.op = op;
        // new_node->data.binary_op.left = *node;
        // new_node->data.binary_op.right = right;
        // *node = new_node;
        token = handle_binary_operation(token, node);
    }
    return token;
}

// Parse an expression: terms with + or -
Token* parse_expression(Token* token, ASTNode** node) {
    token = parse_term(token, node);
    while (token->type == TOKEN_PLUS || token->type == TOKEN_MINUS) {
        // TokenType op = token->type;
        // token = token->next; // Skip operator
        // ASTNode* right;
        // token = parse_term(token, &right);
        // ASTNode* new_node = malloc(sizeof(ASTNode));
        // new_node->type = NODE_BINARY_OP; // //
        // new_node->data.binary_op.op = op;
        // new_node->data.binary_op.left = *node;
        // new_node->data.binary_op.right = right;
        // *node = new_node;
        token = handle_binary_operation(token, node);
    }
    return token;
}

// Evaluate the AST
int evaluate(ASTNode* node) {
    if (node->type == NODE_NUMBER) {
        return node->data.value;
    }
    int left = evaluate(node->data.binary_op.left);
    int right = evaluate(node->data.binary_op.right);
    switch (node->data.binary_op.op) {
        case TOKEN_PLUS: return left + right;
        case TOKEN_MINUS: return left - right;
        case TOKEN_TIMES: return left * right;
        case TOKEN_DIVIDE:
            if (right == 0) {
                printf("Division by zero\n");
                exit(1);
            }
            return left / right;
        default:
            printf("Unknown operator\n");
            exit(1);
    }
    return 0; // Unreachable
}

// Free token list
void free_tokens(Token* token) {
    while (token != NULL) {
        Token* next = token->next;
        free(token);
        token = next;
    }
}

// Free AST
void free_ast(ASTNode* node) {
    if (node == NULL) return;
    if (node->type == NODE_BINARY_OP) {
        free_ast(node->data.binary_op.left);
        free_ast(node->data.binary_op.right);
    }
    free(node);
}

// Print AST for debugging
void print_ast(ASTNode* node, int depth) {
    for (int i = 0; i < depth; i++) printf("  ");
    if (node->type == NODE_NUMBER) {
        printf("NUMBER(%d)\n", node->data.value);
    } else {
        char op = node->data.binary_op.op == TOKEN_PLUS ? '+' :
                  node->data.binary_op.op == TOKEN_MINUS ? '-' :
                  node->data.binary_op.op == TOKEN_TIMES ? '*' : '/';
        printf("OPERATOR(%c)\n", op);
        print_ast(node->data.binary_op.left, depth + 1);
        print_ast(node->data.binary_op.right, depth + 1);
    }
}

// Print tokens for debugging
void print_tokens(Token* token) {
	while (token != NULL) {
		switch (token->type) {
			case TOKEN_NUMBER:
				printf("TOKEN_NUMBER(%d)\n", token->value);
				break;
			case TOKEN_PLUS:
				printf("TOKEN_PLUS(+)\n");
				break;
			case TOKEN_MINUS:
				printf("TOKEN_MINUS(-)\n");
				break;
			case TOKEN_TIMES:
				printf("TOKEN_TIMES(*)\n");
				break;
			case TOKEN_DIVIDE:
				printf("TOKEN_DIVIDE(/)\n");
				break;
			case TOKEN_LPAREN:
				printf("TOKEN_LPAREN(()\n");
				break;
			case TOKEN_RPAREN:
				printf("TOKEN_RPAREN())\n");
				break;
			case TOKEN_EOF:
				printf("TOKEN_EOF\n");
				break;
			default:
				printf("UNKNOWN_TOKEN\n");
				break;
		}
		token = token->next;
	}
}

int main() {
    const char* input = "(2 + 3 * 5) * 4 - 1";
    Token* tokens = tokenize(input);
	// print_tokens(tokens);
    ASTNode* ast;
    Token* end = parse_expression(tokens, &ast);
    if (end->type != TOKEN_EOF) {
        printf("Unexpected tokens at end\n");
        exit(1);
    }
    printf("Abstract Syntax Tree:\n");
    // print_ast(ast, 0);
    printf("Result: %d\n", evaluate(ast));
    free_tokens(tokens);
    free_ast(ast);
    return 0;
}
