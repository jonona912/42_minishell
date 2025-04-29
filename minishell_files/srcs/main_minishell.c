#include "../includes/minishell.h"

volatile int signal_received = 0;

int	check_user_input(char **line)
{
	if (ft_strcmp(*line, "") == 0 || signal_received)
	{
		free(*line);
		*line = NULL;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		return (-1);
	}
	return (0);
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
	(void)envp;
	t_token_lst *token_lst;
	t_token_lst	*token_lst_check;
	t_ast_node	*head;
	int		is_test;
	t_shell shell;

	shell.heredoc_temp_counter = 0;
	shell.last_status = 0;
	shell.env = copy_env(envp);
	if (argc == 2 && ft_strcmp(argv[1], "-test") == 0)
		is_test = 1;
	else
		is_test = 0;
	size_t len = 0;
	head = NULL;
	token_lst = NULL;
	char	*line = NULL;
	// make this a seperate function
	struct sigaction sa;
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	if (is_test)
	{
		// Read input from stdin
		if (getline(&line, &len, stdin) == -1) {
			free(line);
			return 0; // Exit on EOF
		}
		// Remove trailing newline
		line[strcspn(line, "\n")] = 0;
	}
	while (1)
	{
		signal_received = 0;
		if (!is_test)
			line = readline("minishel> ");
		if (!line)
		{
			free(line);
			write(1, "exit\n", 5);
			break;
		}
		if (check_user_input(&line) == -1)
			continue;
		token_lst = ft_tokenize(line);
		if (token_lst)
			add_history(line);
		if (!token_lst || token_lst->type == TOKEN_END)
		{
			// redact later
			free(line);
			shell.last_status = 1;
			// write(1, "\n", 1); // should ot print if nothing was given except
			continue;
		}
		// ft_print_tokens(token_lst);
		token_lst_check = parse_or(token_lst, &head, &shell);
		if (!token_lst_check)
		{
			// printf("exit status = %d\n", shell.last_status);
			// handle error
			// write(1, "Error: parsing failed\n", 22);
			token_free_list(token_lst); // free the double pointer of the token list and set it to NULL cat < input.txt < input_2.txt
			free(line);
			continue ;
		}
		token_free_list(token_lst);
		// print_ast(head);
		// run_pipeline(head);
	
		shell.last_status = execute(head, -1, -1, &shell);
		// printf("exec_result = %d\n", exec_result);
		// printf("exit status = %d\n", shell.last_status);
		rl_on_new_line();
		free_ast_node(head);
		free(line);
		if (is_test)
			break;
	}
	return (0);
}
