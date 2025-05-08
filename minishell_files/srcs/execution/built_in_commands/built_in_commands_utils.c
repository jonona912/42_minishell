/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:54:22 by opopov            #+#    #+#             */
/*   Updated: 2025/05/08 10:04:17 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/built_in_commands.h"

void	handle_no_equal(char **argv, int *exit_status, int *i, t_shell *shell)
{
	if (!is_valid_name(argv[*i]))
	{
		ft_putstr_fd("Error: invalid export input\n", 2);
		*exit_status = 1;
	}
	else
		ft_setenv(argv[*i], "", 0, shell);
	(*i)++;
}

int	return_value_valid(char *name, int *exit_status, int *i)
{
	ft_putstr_fd("Error: invalid variable name\n", 2);
	free(name);
	*exit_status = 1;
	(*i)++;
	return (1);
}

int	return_value_setenv(char *name, int *exit_status)
{
	ft_putstr_fd("Error: invalid syntax input\n", 2);
	free(name);
	*exit_status = 1;
	return (1);
}

int	handle_equal(char **argv, int *exit_status, int *i, t_shell *shell)
{
	char	*equal;
	char	*name;
	int		name_len;

	equal = ft_strchr(argv[*i], '=');
	name_len = equal - argv[*i];
	name = (char *)malloc(name_len + 1);
	if (!name)
	{
		ft_putstr_fd("Error: memory allocation failed\n", 2);
		*exit_status = 1;
		return (1);
	}
	ft_strlcpy(name, argv[*i], name_len + 1);
	if (!is_valid_name(name))
		return (return_value_valid(name, exit_status, i));
	if (ft_setenv(name, equal + 1, 1, shell))
		return (return_value_setenv(name, exit_status));
	free(name);
	(*i)++;
	return (0);
}

int	ft_cd_argc_check(char **argv)
{
	int	argc;

	argc = 0;
	while (argv && argv[argc])
		argc++;
	if (argc > 2)
		return (ft_putstr_fd("Error: too many argunents\n", 2), 1);
	return (0);
}
