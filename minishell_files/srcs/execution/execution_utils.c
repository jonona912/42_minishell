#include "../../includes/minishell.h"

// int	dup2_fd(int fd, int std_fd_fileno)
// {
// 	if (fd == -1)
// 	{
// 		perror("open redirection in");
// 		return (-1);
// 	}
// 	if (dup2(fd, std_fd_fileno) == -1)
// 	{
// 		perror("dup2 redirection in");
// 		return (-1);
// 	}
// 	return (0);
// }

// int	ms_strcmp_until(char *s1, char *s2, char c)
// {
// 	int	i;

// 	i = 0;
// 	if (!s1 || !s2)
// 		return (-1);
// 	while (*(s1 + i) && *(s2 + i) && *(s1 + i) != c && *(s1 + i) == *(s2 + i))
// 		i++;
// 	return (*(s1 + i) - *(s2 + i));
// }

int	builtin_check(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 ||
			ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0 ||
			ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0 ||
			ft_strcmp(cmd, "exit") == 0);
}

int	execute_builtin(char **argv, t_shell *shell)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		ft_echo(argv);
	// else if (ft_strcmp(argv[0], "cd") == 0)
	// 	return (ft_cd(argv, shell));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		ft_pwd(shell);
	else if (ft_strcmp(argv[0], "export") == 0)
		return (ft_export(argv, shell));
	else if (ft_strcmp(argv[0], "unset") == 0)
		return(ft_unset(argv, shell));
	else if (ft_strcmp(argv[0], "env") == 0)
		ft_env(shell);
	else if (ft_strcmp(argv[0], "exit") == 0)
		return (ft_exit(argv));
	return (0);
}
