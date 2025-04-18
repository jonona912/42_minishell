#include "../includes/minishell.h"

void	ft_print_tokens(t_token_lst *token_lst);
void	print_ast(t_ast_node *root);

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

char	**copy_env(char **envp)
{
	int		count = 0;
	char	**copy;
	int		i;

	while (envp[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	(void)argc;
	t_shell shell;
	int prev_status;

	shell.env = copy_env(envp);
	// debug_env(&shell);
	// printf("\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
	shell.last_status = 0;
	prev_status = 0;
	t_token_lst *token_lst;
	t_token_lst	*token_lst_check;
	t_ast_node *head;
	int			exec_result;

	exec_result = 0;
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
		pipe(shell.exp_pipe); // shell.exp_pipe[1] -> for write side
		signal_received = 0;
		// line = readline("\033[0;35mminishell> \033[0m");
		line = readline("minishel> ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break;
		}
		add_history(line);
		if (ft_strcmp(line, "") == 0)
		{
			free(line);
			continue;
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
		// wildcard_function(line);
		add_history(line);
		token_lst_check = parse_or(token_lst, &head, &shell.last_status);
		// print_ast(head);
		if (!token_lst_check)
			exec_result = 127;
		else
			exec_result = execute(head, -1, -1, &shell);
		// print_ast(head);
		// debug_env(&shell);
		// printf("\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
		// run_pipeline(head);

		// printf("exec_result = %d\n", exec_result);

		// ms_token_free_list(token_lst);

		rl_on_new_line();
		free(line);
	}
	return (0);
}

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
