#include "../includes/built_in_commands.h"

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
	ft_echo_print_loop(argv, &i);
	if (!n)
		printf("\n");
}

void	ft_env(t_shell *shell)
{
	int		i;
	char	*tmp;

	i = 0;
	while (shell->env[i])
	{
		tmp = ft_strchr(shell->env[i], '=');
		if (tmp && tmp[1])
		{
			write(1, shell->env[i], ft_strlen(shell->env[i]));
			write(1, "\n", 1);
		}
		i++;
	}
}

int	ft_cd(char **argv, t_shell *shell)
{
	char		cwd[4096];
	static char	*oldpwd = NULL;
	char		new_cwd[4096];
	char		*tmp;

	if (argv[2] && argv[1])
		return (printf("Error: too many argunemtns\n"), 1);
	if (ft_cd_special_cases(argv, shell, &tmp, oldpwd))
		return (1);
	if (!getcwd(cwd, sizeof(cwd)))
		return (ft_putstr_fd
			("Error: current working directory name not found\n", 2), 1);
	if (chdir(tmp) != 0)
		return (ft_putstr_fd
			("Error: directory cannot be changed\n", 2), 1);
	if (ft_setenv("OLDPWD", cwd, 1, shell))
		return (1);
	if (!getcwd(new_cwd, sizeof(new_cwd)))
		return (ft_putstr_fd
			("Error: cannot set PWD to new directory\n", 2), 1);
	if (ft_setenv("PWD", new_cwd, 1, shell))
		return (1);
	if (oldpwd)
		free(oldpwd);
	oldpwd = ft_strdup(cwd);
	return (0);
}

int	ft_export(char **argv, t_shell *shell)
{
	char	*equal;
	char	*name;
	int		name_len;
	int		i;
	int		exit_status;

	if (!argv[1])
		return (ft_export_env(*shell), 0);
	i = 1;
	exit_status = 0;
	while (argv[i])
	{
		equal = ft_strchr(argv[i], '=');
		if (!equal)
		{
			if (!is_valid_name(argv[i]))
			{
				printf("Error: invalid export input\n");
				exit_status = 1;
			}
			else
				ft_setenv(argv[i], "", 0, shell);
			i++;
			continue ;
		}
		name_len = equal - argv[i];
		name = (char *) malloc(name_len + 1);
		if (!name)
			return (printf("Error: memory allocation failed\n"), 1);
		ft_strlcpy(name, argv[i], name_len + 1);
		if (!is_valid_name(name))
		{
			printf("Error: invalid variable name\n");
			free(name);
			exit_status = 1;
			i++;
			continue ;
		}
		if (ft_setenv(name, equal + 1, 1, shell))
		{
			free(name);
			return (printf("Error: invalid syntax input\n"), 1);
		}
		free(name);
		i++;
	}
	return (exit_status);
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
