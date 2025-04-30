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

int	handle_heredoc(char *end_delimitor, int in_fd) // should hangle signal
{
	char	*input;

	if (!end_delimitor)
		return (-1);
	while (1)
	{
		input = readline("\033[0;35mheredoc>\033[0m");
		printf("heredoc input = %s\n", input);
		if (ms_strcmp_until(end_delimitor, input, '\n') == 0)
		{
			free(input);
			break ;
		}
		ft_putstr_fd(input, in_fd);
		ft_putstr_fd("\n", in_fd);
		free(input);
	}
	return (1);
}

int	run_heredoc_error_handler(int *pipe_fd, char *end_delimitor)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	if (handle_heredoc(end_delimitor, pipe_fd[1]) == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (0);
	}
	return (1);
}

int	run_heredoc(char *end_delimitor, int *in_fd)
{
	int		pipe_fd[2];
	int		is_interprete;
	char	*temp;

	temp = end_delimitor;
	is_interprete = 1;
	if (end_delimitor == NULL)
		return (-1);
	if (*end_delimitor == '\"' || *end_delimitor == '\'')
	{
		end_delimitor = ft_strtrim(end_delimitor, "\"\'");
		if (!end_delimitor)
			return (-1);
		is_interprete = 0;
	}
	if (!run_heredoc_error_handler(pipe_fd, end_delimitor))
		return (-1);
	close(pipe_fd[1]);
	*in_fd = pipe_fd[0];
	if (!is_interprete)
		free(end_delimitor);
	return (0);
}
