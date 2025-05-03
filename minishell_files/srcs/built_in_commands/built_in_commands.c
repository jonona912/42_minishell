#include "../../includes/minishell.h"

void	ft_echo(char **argv)
{
	int	i;
	int	n;

	n = 0;
	i = 1;
	if (!argv[i])
		return ;
	while (argv[i] && is_n_flag(argv[i]))
	{
		n = 1;
		i++;
	}
	ft_echo_loop(argv, &i);
	if (!n)
		printf("\n");
}

void	ft_env(t_shell *shell, int is_exp)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		if (is_exp)
			write(1, "declared -x ", 12);
		write(1, shell->env[i], ft_strlen(shell->env[i]));
		write(1, "\n", 1);
		i++;
	}
}

int	ft_cd(char **argv, t_shell *shell)
{
	char		cwd[4096];
	static char	*oldpwd = NULL;
	char		*curr_pwd;
	char		*tmp;

	if (argv[2])
		return (printf("Error: too many arguments\n"), 1);
	if (ft_cd_special_cases(argv, shell, &tmp, oldpwd))
		return (1);
	if (getcwd(cwd, sizeof(cwd)))
		curr_pwd = cwd;
	else
	{
		curr_pwd = ft_getenv("PWD", *shell);
		if (!curr_pwd)
			curr_pwd = "";
	}
	if (chdir(tmp) != 0)
		return (printf("Error: directory cannot be changed\n"), 1);
	if (ft_cd_end(curr_pwd, shell, &oldpwd, cwd))
		return (1);
	return (0);
}

int	ft_export(char **argv, t_shell *shell)
{
	char	*equal;
	char	*name;
	int		name_len;
	// (void)shell;
	// int		i;

	// i = 0;
	// while (argv[i])
	// {
	// 	printf("DEBUG: %s\n", argv[i]);
	// 	i++;
	// }
	if (!argv[1])
		return (ft_env(shell, 1), 0);
	equal = ft_strchr(argv[1], '=');
	if (!equal)
		return (printf("Error: invalid export input\n"), 1);
	name_len = equal - argv[1];
	name = (char *) malloc(name_len + 1);
	if (!name)
		return (printf("Error: memory allocation failed\n"), 1);
	ft_strlcpy(name, argv[1], name_len + 1);
	if (!is_valid_name(name))
		return (free(name), printf("Error: invalid variable name\n"), 1);
	if (ft_setenv(name, equal + 1, 1, shell))
	{
		free(name);
		return (printf("Error: invalid syntax input\n"), 1);
	}
	free(name);
	return (0);
}

int	ft_unset(char **argv, t_shell *shell)
{
	int			i;
	char		*equal;
	int			j;

	i = 0;
	if (!argv[1])
		return (ft_putstr_fd("Error: missing argument for unset\n", 2), 1);
	while (shell->env[i])
	{
		equal = ft_strchr(shell->env[i], '=');
		if (equal && ft_strncmp
			(shell->env[i], argv[1], equal - shell->env[i]) == 0)
		{
			free(shell->env[i]);
			j = i;
			while (shell->env[j])
			{
				shell->env[j] = shell->env[j + 1];
				j++;
			}
			return (0);
		}
		i++;
	}
	return (0);
}
