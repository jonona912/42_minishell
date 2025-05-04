#include "includes/execution.h"

int	builtin_check(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "env") == 0);
}

int	execute_builtin(char **argv, t_shell *shell) // handle $P
{
	if (ft_strcmp(argv[0], "echo") == 0)
		ft_echo(argv);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(argv[0], "env") == 0)
		ft_env(shell);
	return (0);
}
