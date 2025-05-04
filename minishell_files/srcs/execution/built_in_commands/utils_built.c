#include "../includes/built_in_commands.h"

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

void	ft_echo_print_loop(char **argv, int *i)
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

int	is_valid_name(char *name)
{
	int	i;

	i = 0;
	if (!name || !*name)
		return (0);
	if (!(ft_isalpha(name[i]) || name[i] == '_'))
		return (0);
	i++;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}
