#include "../../includes/minishell.h"

int	ms_strcmp_until(char *s1, char *s2, char c)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (*(s1 + i) && *(s2 + i) && *(s1 + i) != c && *(s1 + i) == *(s2 + i))
		i++;
	return (*(s1 + i) - *(s2 + i));
}

void	heredoc_signal_handler(int signum)
{
	(void)signum;
	g_signal_received = 1;
	ioctl(0, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_done = 1;
}

int	heredoc_loop(char *end_delimitor, int in_fd, struct sigaction *old_sa)
{
	char	*input;

	while (1)
	{
		input = readline("\033[0;35mheredoc>\033[0m");
		if (g_signal_received)
			return (sigaction(SIGINT, old_sa, NULL), -1);
		if (!input)
		{
			sigaction(SIGINT, old_sa, NULL);
			ft_putstr_fd("warning: here-doc delimited by end-of-file\n", 2);
			return (1);
		}
		if (ms_strcmp_until(end_delimitor, input, '\n') == 0)
		{
			free(input);
			break ;
		}
		ft_putstr_fd(input, in_fd);
		ft_putstr_fd("\n", in_fd);
		free(input);
	}
	sigaction(SIGINT, old_sa, NULL);
	return (0);
}

int	handle_heredoc(char *end_delimitor, int in_fd)
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
	return (heredoc_loop(end_delimitor, in_fd, &old_sa));
}
