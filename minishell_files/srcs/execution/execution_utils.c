/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:47:17 by opopov            #+#    #+#             */
/*   Updated: 2025/04/26 17:48:31 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_check(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int	execute_builtin(char **argv, t_shell *shell)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		ft_echo(argv);
	else if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_cd(argv, shell));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		ft_pwd(shell);
	else if (ft_strcmp(argv[0], "export") == 0)
		return (ft_export(argv, shell));
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (ft_unset(argv, shell));
	else if (ft_strcmp(argv[0], "env") == 0)
		ft_env(shell);
	else if (ft_strcmp(argv[0], "exit") == 0)
		return (ft_exit(argv));
	return (0);
}
