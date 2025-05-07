/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:38:54 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 18:30:21 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (ft_putstr_fd("Error: too many argunents\n", 2), 1);
	if (ft_cd_special_cases(argv, shell, &tmp, oldpwd))
		return (1);
	if (!getcwd(cwd, sizeof(cwd)))
		return (ft_putstr_fd
			("Error: current working directory name not found\n", 2), 1);
	if (chdir(tmp) != 0)
		return (ft_putstr_fd ("Error: directory cannot be changed\n", 2), 1);
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
	int		i;
	int		exit_status;
	char	*equal;

	if (!argv[1])
		return (ft_export_env(*shell), 0);
	i = 1;
	exit_status = 0;
	while (argv[i])
	{
		equal = ft_strchr(argv[i], '=');
		if (!equal)
			handle_no_equal(argv, &exit_status, &i, shell);
		else
			handle_equal(argv, &exit_status, &i, shell);
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
		return (0);
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
