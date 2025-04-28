/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:13:52 by opopov            #+#    #+#             */
/*   Updated: 2025/04/28 18:29:49 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_echo(char **argv, t_shell *shell)
{
	int	i;
	int	n;

	n = 0;
	i = 1;
	if (!argv[i])
		return ;
	while (ft_strncmp(argv[i], "-n", 2) == 0) //  echo -nnnn -nff
	{
		if (!argv[i + 1])
			return ;
		n = 1;
		i++;
	}
	ft_echo_loop(argv, shell, &i);
	if (!n)
		printf("\n");
}

void	ft_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		write(1, shell->env[i], ft_strlen(shell->env[i]));
		write(1, "\n", 1);
		i++;
	}
}

int	ft_cd(char **argv, t_shell *shell)
{
	char		cwd[4096];
	static char	*oldpwd = NULL;
	char		new_cwd[4096];
	char		*tmp;

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

	if (!argv[1])
	{
		ft_env(shell);
		return (0);
	}
	equal = ft_strchr(argv[1], '=');
	if (!equal || equal == argv[1])
		return (perror("Error: invalid syntax input"), 1);
	name_len = equal - argv[1];
	name = (char *) malloc(name_len + 1);
	if (!name)
		return (perror("Error: memory allocation failed"), 1);
	ft_strlcpy(name, argv[1], name_len + 1);
	if (ft_setenv(name, equal + 1, 1, shell))
	{
		perror("Error: invalid syntax input");
		free(name);
		return (1);
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
