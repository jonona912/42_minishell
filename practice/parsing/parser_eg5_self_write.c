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

Token *parse_factor(Token *token, ASTNode **node)
{
	if (token->type == TOKEN_NUMBER)
	{
		*node = malloc(sizeof(ASTNode));
		(*node)->type = NODE_NUMBER;
		(*node)->data.value = token->value;
		return (token->next); // returns if it is number
	}
	else if (token->type == TOKEN_LPAREN)
	{
		token = token->next; // skip (
		token = parse_expression(token, node);
        if (token->type != TOKEN_RPAREN)
        {
            printf("right parenthesis error\n");
            exit(1);
        }
        return (token->next);
		// return (token);
	}
    else
    {
        printf("expected num or (\n");
        exit (1);
    }
	// return (token);
}

Token *handle_binary_operation(Token* token, ASTNode **node)
{
    TokenType operation = token->type;
    token = token->next;
    ASTNode *right = NULL;
    token = parse_term(token, &right);
    ASTNode *temp = (ASTNode *)malloc(sizeof(ASTNode));
    temp->type = NODE_BINARY_OP;
    temp->data.binary_op.left = *node;
    temp->data.binary_op.right = right;
    *node = temp;
    return (token);
}

Token *parse_term(Token *token, ASTNode **node)
{
	token = parse_factor(token, node);
	while (token->type == TOKEN_DIVIDE || token->type == TOKEN_TIMES)
	{
        token = handle_binary_operation(token, node);
	}
    return (token);
}


Token *parse_expression(Token *token, ASTNode **node)
{
	token = parse_term(token, node);
    while (token->type == TOKEN_PLUS || token->type == TOKEN_MINUS)
    {
        token = handle_binary_operation(token, node);
    }
    return (token);
}

int evaluate(ASTNode *node)
{
    if (node->type == NODE_NUMBER)
        return (node->data.value);
    int left = evaluate(node->data.binary_op.left);
    int right = evaluate(node->data.binary_op.right);
    if (node->data.binary_op.op == TOKEN_DIVIDE)
        return (left + right);
}

int main()
{
	const char *input = "(2 + 3) * 4 - 1";
	Token *tokens = tokenize(input);
	ASTNode *ast = NULL;
	Token *end = parse_expression(tokens, &ast);

	return (0);
}