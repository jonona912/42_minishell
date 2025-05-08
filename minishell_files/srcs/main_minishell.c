/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:33:08 by opopov            #+#    #+#             */
/*   Updated: 2025/05/08 13:19:47 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	*token_lst = ft_tokenize(*line, shell);
	if (!*token_lst || (*token_lst)->type == TOKEN_END)
		return (free(*line), 1);
	if (!is_first_token_valid((*token_lst)->type))
	{
		token_free_list(*token_lst);
		*head = NULL;
		shell->last_status = 2;
		free(*line);
		return (ft_putstr_fd("minishell:: syntax error\n", 2), 1);
	}
	if (!parse_or(*token_lst, head, shell))
	{
		token_free_list(*token_lst);
		if (head)
			free_ast_node(head);
		return (free(*line), 1);
	}
	g_signal_received = 1;
	shell->last_status = ex(*head, -1, -1, shell);
	g_signal_received = 0;
	free_ast_node(head);
	token_free_list(*token_lst);
	free(*line);
	return (0);
}

int	ft_update_path(t_shell *shell)
{
	char	cwd[4096];
	char	*old_pwd;

	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_setenv("PWD", cwd, 1, shell);
		return (0);
	}
	old_pwd = ft_getenv("OLDPWD", *shell);
	if (!old_pwd)
		return (ft_putstr_fd("Error: OLDPWD not set\n", 2), 1);
	if (chdir(old_pwd))
	{
		ft_putstr_fd("Errror: Can't get back to OLDPWD\n", 2);
		return (1);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd("Error: problem with recovered path\n", 2);
		ft_setenv("PWD", old_pwd, 1, shell);
	}
	else
		ft_setenv("PWD", cwd, 1, shell);
	return (0);
}

void	main_loop(t_shell *shell)
{
	char		*line;
	t_token_lst	*token_lst;
	t_ast_node	*head;

	token_lst = NULL;
	head = NULL;
	while (1)
	{
		g_signal_received = 0;
		line = readline("minishell> ");
		if (!line)
		{
			rl_clear_history();
			printf("exit\n");
			free(line);
			break ;
		}
		if (*line != '\0')
			add_history(line);
		ft_update_path(shell);
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
	env_free(shell.env);
	return (0);
}
