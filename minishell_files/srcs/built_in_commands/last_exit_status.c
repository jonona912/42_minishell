
#include "../../includes/minishell.h"

int	return_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			return (130);
		else if (WTERMSIG(status) == SIGQUIT)
			return (131);
	}
	else if (WIFSTOPPED(status))
	{
		if (WSTOPSIG(status) == SIGTSTP)
			return (126);
		else if (WSTOPSIG(status) == SIGTTIN)
			return (127);
	}
	else if (WIFCONTINUED(status))
		return (128);
	return (status);
}

void	set_and_move_eight_bits_left(int *x, int set_num)
{
	*x = set_num;
	*x = (*x << 8);
}
