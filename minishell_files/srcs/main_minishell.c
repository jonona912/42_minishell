#include "../includes/minishell.h"

void	signal_handler(int signum)
{
	(void)signum;
	if (g_signal_received)
		return ;
	rl_replace_line("", 0);
	write(1, "\r\n", 2);
	rl_on_new_line();
	rl_redisplay();
}

int	main_loop_tokenize_parse_execute(char **line, t_shell *shell,
	t_token_lst **token_lst, t_ast_node **head)
{
	*token_lst = ft_tokenize(*line);
	if (!*token_lst || (*token_lst)->type == TOKEN_END)
	{
		free(*line);
		shell->last_status = 1;
		return (1);
	}
	if (!parse_or(*token_lst, head, shell))
	{
		token_free_list(*token_lst);
		if (head)
			free_ast_node(head);
		free(*line);
		return (1);
	}
	g_signal_received = 1;
	shell->last_status = execute(*head, -1, -1, shell);
	g_signal_received = 0;
	free_ast_node(head);
	token_free_list(*token_lst);
	free(*line);
	return (0);
}

void	main_loop(t_shell *shell)
{
	char		*line;
	t_token_lst	*token_lst;
	t_ast_node	*head;

	token_lst = NULL;
	line = NULL;
	head = NULL;
	while (1)
	{
		g_signal_received = 0;
		line = readline("minishell> ");
		if (!line)
		{
			rl_clear_history();
			printf("exit\n");
			break ;
		}
		if (*line != '\0')
			add_history(line);
		if (main_loop_tokenize_parse_execute(&line, shell, &token_lst, &head))
			continue ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell				shell;
	struct sigaction	sa_int;

	(void)argv;
	(void)argc;
	shell.heredoc_ctr = 0;
	shell.last_status = 0;
	shell.env = copy_env(envp);
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sa_int.sa_handler = signal_handler;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
	main_loop(&shell);
	shell_env_free(&shell);
	return (0);
}
