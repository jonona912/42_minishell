#include "../../includes/minishell.h"

int	ft_cd_special_cases(char **argv, t_shell *shell,
		char **tmp, char *oldpwd)
{
	*tmp = argv[1];
	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
	{
		*tmp = ft_getenv("HOME", *shell);
		if (!*tmp)
		{
			ft_putstr_fd("Error: HOME is not set\n", 2);
			return (1);
		}
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		if (oldpwd)
			*tmp = oldpwd;
		else
			*tmp = ft_getenv("OLDPWD", *shell);
		if (!*tmp)
		{
			ft_putstr_fd("Error: OLDPWD is not set\n", 2);
			return (1);
		}
	}
	return (0);
}

void	ft_pwd(t_shell *shell)
{
	char	*pwd;

	check_cwd(shell);
	pwd = ft_getenv("PWD", *shell);
	if (pwd)
		printf("%s\n", pwd);
	else
		ft_putstr_fd("Error: path is not found\n", 2);
}

int	is_n_flag(char *argv)
{
	int	i;

	if (!argv || argv[0] != '-')
		return (0);
	i = 1;
	while (argv[i])
	{
		if (argv[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo_loop(char **argv, int *i)
{
	int	j;

	while (argv[*i])
	{
		j = 0;
		while (argv[*i][j])
		{
			printf("%c", argv[*i][j]);
			j++;
		}
		if (argv[*i + 1])
			printf(" ");
		(*i)++;
	}
}

int	ft_exit(char **argv)
{
	int	i;

	i = 0;
	if (argv[2])
		return (printf("exit: too many arguments\n"), 1);
	if (argv[1])
	{
		while (argv[1][i])
		{
			if (ft_isalpha(argv[1][i]))
			{
				printf("exit: %s: numeric argument required\n", argv[1]);
				exit(2);
			}
			i++;
		}
		printf("exit\n");
		exit(ft_atoi(argv[1]));
	}
	printf("exit\n");
	exit(0);
}
