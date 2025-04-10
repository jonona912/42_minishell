/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 14:01:59 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/10 15:48:31 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
void	ft_print_tokens(t_token_lst *token_lst);

// int	ms_count_argv(t_token_lst *token_lst)
// {
// 	int	count;

// 	count = 0;
// 	while (token_lst && ft_strcmp(token_lst->value, "|") != 0)
// 	{
// 		count++;
// 		token_lst = token_lst->next;
// 	}
// 	return (count);
// }

// t_cmd_lst	*ms_create_cmd_lst(t_token_lst *token_lst) // forget about pipes for now
// {
// 	t_cmd_lst	*cmd_lst;
// 	int			argv_count;
// 	int			i;
// 	char		*path;
// 	char		**temp_argv;
// 	i = 0;

// 	cmd_lst = NULL;
// 	if (!token_lst)
// 		return (NULL);
// 	argv_count = ms_count_argv(token_lst);
// 	temp_argv = (char **)malloc(sizeof(char *) * (argv_count + 1));
// 	if (!temp_argv)
// 		return (NULL); // improve
// 	if (i == 0)
// 		path = ft_strjoin("/bin/", token_lst->value);
// 	while (token_lst && ft_strcmp(token_lst->value, "|") != 0)
// 	{
// 		temp_argv[i] = ft_strdup(token_lst->value);
// 		if (!temp_argv[i])
// 			return (NULL); // improve
// 		i++;
// 		token_lst = token_lst->next;
// 	}
// 	temp_argv[i] = NULL;
// 	t_cmd_lst *new_node = ms_new_node(path, temp_argv);
// 	if (!new_node)
// 		return (NULL); // improve
// 	ms_add_node_back(&cmd_lst, new_node);
// 	return (cmd_lst);
// }

// int	execute_cmd(t_cmd_lst *cmd_lst)
// {
// 	int		status;
// 	pid_t	pid;

// 	status = 0;
// 	if (!cmd_lst)
// 		return (-1);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		if (execve(cmd_lst->path, cmd_lst->av, NULL) == -1)
// 		{
// 			perror("execve");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	else if (pid < 0)
// 	{
// 		perror("fork");
// 		return (1);
// 	}
// 	else
// 		waitpid(pid, &status, 0);
// 	return (status);
// }

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
	// t_cmd_lst	*cmd_lst;

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
		line = readline("minishell> ");
		if (!line)
		{
			write(1, "exit\n", 5);
			free(line);
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
			write(1, "\n", 1);
			continue;
		}
		// next step -->
		// token_lst = token_lst_exansion(token_lst);
		ft_print_tokens(token_lst);
		// expansion
		// cmd_lst = ms_create_cmd_lst(token_lst);
		// ms_token_free_list(token_lst);
		// execute_cmd(cmd_lst);
		// add_history(line);
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
