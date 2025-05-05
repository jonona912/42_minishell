#include "../../includes/minishell.h"

void	ft_print_tokens(t_token_lst *token_lst)
{
	const char *token_type_str[] = {
		"TOKEN_WORD",
		"TOKEN_S_QUOTE",
		"TOKEN_D_QUOTE",
		"TOKEN_WILDCARD",
		"TOKEN_PIPE",
		"TOKEN_REDIRECTION_IN",
		"TOKEN_REDIRECTION_OUT",
		"TOKEN_APPEND",
		"TOKEN_HEREDOC",
		"TOKEN_ENV_VAR",
		"TOKEN_AND",
		"TOKEN_OR",
		"TOKEN_L_PAREN",
		"TOKEN_R_PAREN",
		"TOKEN_END"
	};

	while (token_lst)
	{
		printf("Type : %s\nToken: \"%s\"\n\n", token_type_str[token_lst->type], token_lst->value);
		token_lst = token_lst->next;
	}
}

void print_redirections(t_redir_lst *redirs) {
    if (!redirs) return;
    printf(" redirs=[");
    while (redirs) {
        printf("%s = %s",
               redirs->type == TOKEN_REDIRECTION_IN ? "<" :
               redirs->type == TOKEN_REDIRECTION_OUT ? ">" :
               redirs->type == TOKEN_APPEND ? ">>" : "<<",
               redirs->target);
        redirs = redirs->next;
        if (redirs) printf(", ");
    }
    printf("]");
}

void print_ast_node(t_ast_node *node, int indent)
{
    if (!node) return;

    // Indentation
    for (int i = 0; i < indent; i++) printf("  ");

    switch (node->type) {
        case NODE_CMD:
            printf("CommandNode(");
            printf("%s", node->data.cmd.executable);

            // Print arguments if they exist
            if (node->data.cmd.exec_argv && node->data.cmd.exec_argv[0]) {
                printf(", args=[");
                for (char **arg = node->data.cmd.exec_argv; *arg; arg++) {
                    printf("%s", *arg);
                    if (*(arg + 1)) printf(", ");
                }
                printf("]");
            }

            // Print redirections
            print_redirections(node->data.cmd.redirs);
            printf(")");
            break;

        case NODE_PIPE:
            printf("PipeNode(\n");
            print_ast_node(node->data.binary_op.left, indent + 1);
            printf(",\n");
            print_ast_node(node->data.binary_op.right, indent + 1);
            printf("\n");
            for (int i = 0; i < indent; i++) printf("  ");
            printf(")");
            break;

        case NODE_AND:
            printf("AndNode(\n");
            print_ast_node(node->data.binary_op.left, indent + 1);
            printf(",\n");
            print_ast_node(node->data.binary_op.right, indent + 1);
            printf("\n");
            for (int i = 0; i < indent; i++) printf("  ");
            printf(")");
            break;

        case NODE_OR:
            printf("OrNode(\n");
            print_ast_node(node->data.binary_op.left, indent + 1);
            printf(",\n");
            print_ast_node(node->data.binary_op.right, indent + 1);
            printf("\n");
            for (int i = 0; i < indent; i++) printf("  ");
            printf(")");
            break;
        case NODE_SUBSHELL:
            printf("SubshellNode(\n");
            print_ast_node(node->data.sub_shell.subshell, indent + 1);
            printf(",\n");
            print_redirections(node->data.sub_shell.sub_shell_redir);
            printf("\n");
            for (int i = 0; i < indent; i++) printf("  ");
            printf(")");
            break;
    }
}

// Wrapper function for cleaner output
void print_ast(t_ast_node *root)
{
    print_ast_node(root, 0);
    printf("\n");
}
