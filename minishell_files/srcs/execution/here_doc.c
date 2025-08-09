/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:57:04 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 15:12:52 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/execution.h"

void	heredoc_signal_handler(int signum)
{
	(void)signum;
	g_signal_received = 1;
	ioctl(0, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_done = 1;
}

int	read_heredoc_input(char *end_delimitor, int in_fd,
	struct sigaction *old_sa, char **tmp_file)
{
	char	*input;

	while (1)
	{
		input = readline("\033[0;35mheredoc>\033[0m");
		if (g_signal_received)
		{
			free(input);
			input = NULL;
			free(*tmp_file);
			*tmp_file = NULL;
			return (sigaction(SIGINT, old_sa, NULL), -1);
		}
		if (!input)
			return (handle_heredoc_eof(old_sa));
		if (ms_strcmp_until(end_delimitor, input, '\n') == 0)
		{
			free(input);
			break ;
		}
		handle_heredoc_input(input, in_fd);
	}
	sigaction(SIGINT, old_sa, NULL);
	return (0);
}

int	handle_heredoc(char *end_delimitor, int in_fd, char **tmp_file)
{
	struct sigaction	sa;
	struct sigaction	old_sa;

	if (!end_delimitor)
		return (-1);
	g_signal_received = 0;
	sa.sa_handler = heredoc_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &old_sa);
	return (read_heredoc_input(end_delimitor, in_fd, &old_sa, tmp_file));
}
