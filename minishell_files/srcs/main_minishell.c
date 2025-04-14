/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 14:01:59 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/14 23:17:17 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
void	ft_print_tokens(t_token_lst *token_lst);
void print_ast(t_ast_node *root);

volatile int signal_received = 0;

void setup_terminal()
{
	struct termios term;
	tcgetattr(0, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &term);
}

void signal_handler(int signum)
{
	(void)signum;
	signal_received = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(void)
{
	t_token_lst *token_lst;
	t_ast_node *head;

	head = NULL;
	char	*line;
	struct sigaction sa;
	setup_terminal();
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	token_lst = NULL;
	while (1)
	{
		signal_received = 0;
		// line = readline("\033[0;35mminishell> \033[0m");
		line = readline("minishel> ");
		add_history(line);
		if (ft_strcmp(line, "") == 0)
		{
			free(line);
			continue;
		}
		if (!line)
		{
			write(1, "exit\n", 5);
			break;
		}
		if (signal_received)
		{
			free(line);
			continue;
		}
		token_lst = ft_tokenize(line);
		if (!token_lst)
		{
			// redact later
			free(line);
			write(1, "\n", 1);
			continue;
		}
		// ft_print_tokens(token_lst);
		parse_or(token_lst, &head);
		print_ast(head);
		int exec_result = run_pipeline(head);
		printf("exec_result = %d\n", exec_result);
		
		// ms_token_free_list(token_lst);
		add_history(line);
		rl_on_new_line();
		free(line);
	}
	return (0);
}

void	ft_print_tokens(t_token_lst *token_lst)
{
	const char *token_type_str[] = {
		"TOKEN_WORD",
		"TOKEN_PIPE",
		"TOKEN_REDIRECTION_IN",
		"TOKEN_REDIRECTION_OUT",
		"TOKEN_APPEND",
		"TOKEN_HEREDOC",
		"TOKEN_ENV_VAR",
		"TOKEN_S_QUOTE",
		"TOKEN_D_QUOTE",
		"TOKEN_WILDCARD",
		"TOKEN_AND",
		"TOKEN_OR",
		"TOKEN_L_PAREN",
		"TOKEN_R_PAREN"
	};

	while (token_lst)
	{
		printf("Type : %s\nToken: \"%s\"\n\n", token_type_str[token_lst->type], token_lst->value);
		token_lst = token_lst->next;
	}
}

// void	print_ast(t_ast_node *node)
// {
//     if (!node)
//         return;

//     // If the node is a binary operation, traverse left, print the operator, then traverse right
//     if (node->type == NODE_PIPE)
//     {
//         print_ast(node->data.binary_op.left);
//         printf("PIPE\n");
//         print_ast(node->data.binary_op.right);
//     }
//     else if (node->type == NODE_CMD)
//     {
//         // Print the command and its arguments
//         printf("CMD: %s\n", node->data.cmd.executable);
//         if (node->data.cmd.exec_argv)
//         {
//             for (int i = 0; node->data.cmd.exec_argv[i]; i++)
//                 printf("ARG[%d]: %s\n", i, node->data.cmd.exec_argv[i]);
//         }
//     }
// }

#include <stdio.h>
#include <stdlib.h>

void print_redirections(t_redir_lst *redirs) {
    if (!redirs) return;
    printf(" redirs=[");
    while (redirs) {
        printf("%s->%s", 
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
    }
}

// Wrapper function for cleaner output
void print_ast(t_ast_node *root) {
    print_ast_node(root, 0);
    printf("\n");
}