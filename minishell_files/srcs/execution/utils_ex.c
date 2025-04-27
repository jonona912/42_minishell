/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:41:28 by opopov            #+#    #+#             */
/*   Updated: 2025/04/27 11:31:25 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (-1);
}

void	handle_subshell_if(t_ast_node *ast_head, int in_fd,
		int out_fd, t_shell *shell)
{
	int	status;

	if (handle_redirection_fd
		(ast_head->data.sub_shell.sub_shell_redir, &in_fd))
		exit(1);
	if (in_fd != -1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	status = execute(ast_head->data.sub_shell.subshell, -1, -1, shell);
	exit(status);
}

void	execute_cmd_parent_free(t_shell *shell, char **new_env)
{
	int	i;

	if (shell->env)
	{
		i = 0;
		while (shell->env[i] != NULL)
		{
			free(shell->env[i]);
			i++;
		}
		free(shell->env);
	}
	shell->env = new_env;
}

int	execute_cmd_parent_loop_if(char **new_env, int *i, int *pipe_fd, int *len)
{
	if (!new_env[*i] || read(pipe_fd[0], new_env[*i], *len) != *len)
	{
		if (new_env[*i])
			free(new_env[*i]);
		while ((*i)--)
			free(new_env[*i]);
		free(new_env);
		close(pipe_fd[0]);
		return (0);
	}
	return (1);
}

int	execute_cmd_parent_loop(int count, char **new_env, int *pipe_fd)
{
	int	i;
	int	len;

	i = 0;
	while (i < count)
	{
		if (read(pipe_fd[0], &len, sizeof(len)) != sizeof(len))
		{
			while (i--)
				free(new_env[i]);
			free(new_env);
			close(pipe_fd[0]);
			return (0);
		}
		new_env[i] = (char *)malloc(len);
		if (!execute_cmd_parent_loop_if(new_env, &i, pipe_fd, &len))
			return (0);
		i++;
	}
	new_env[count] = NULL;
	return (1);
}
