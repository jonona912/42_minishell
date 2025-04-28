#include "../../includes/minishell.h"

int	ft_cd_special_cases(char **argv, t_shell *shell,
		char **tmp, char *oldpwd)
{
	*tmp = argv[1];
	if (!argv[1] || strcmp(argv[1], "~") == 0)
	{
		*tmp = ft_getenv("HOME", *shell);
		if (!*tmp)
		{
			ft_putstr_fd("Error: HOME is not set\n", 2);
			return (1);
		}
	}
	else if (strcmp(argv[1], "-") == 0)
	{
		if (oldpwd)
			*tmp = oldpwd;
		else
			*tmp = ft_getenv("OLDPWD", *shell);
		if (!tmp)
		{
			ft_putstr_fd("Error: OLDPWD is not set\n", 2);
			return (1);
		}
	}
	return (0);
}

void	ft_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		ft_putstr_fd("Error: path is not found\n", 2);
}

void	ft_echo_loop(char **argv, t_shell *shell, int *i)
{
	int	j;

	while (argv[*i])
	{
		j = 0;
		while (argv[*i][j])
		{
			if (argv[*i][j] == '$' && argv[*i][j + 1] == '?')
			{
				printf("%d", shell->last_status);
				j += 2;
				continue ;
			}
			else
				printf("%c", argv[*i][j]);
			j++;
		}
		if (argv[*i + 1])
			printf(" ");
		(*i)++;
	}
}

// int	ft_exit(char **argv)
// {
// 	if (argv[1])
// 	{
// 		write(1, "Error: too many arguments\n", 26);
// 		return (1);
// 	}
// 	exit(0);
// }
