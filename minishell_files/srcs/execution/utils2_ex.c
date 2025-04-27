/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_ex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:29:05 by opopov            #+#    #+#             */
/*   Updated: 2025/04/27 11:31:22 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_cmd_child_builtin_loop(int count, int *pipe_fd, t_shell *shell)
{
	int	i;
	int	len;

	i = 0;
	while (i < count)
	{
		len = ft_strlen(shell->env[i]) + 1;
		if (write(pipe_fd[1], &len, sizeof(len)) == -1
			|| write(pipe_fd[1], shell->env[i], len) == -1)
		{
			close(pipe_fd[1]);
			exit(1);
		}
		i++;
	}
}

void	execute_cmd_child_beginning(int *pipe_fd,
	t_ast_node *ast_node, int in_fd)
{
	close(pipe_fd[0]);
	if (ast_node->data.cmd.redirs
		&& is_redirection(ast_node->data.cmd.redirs->type))
	{
		if (handle_redirection_fd(ast_node->data.cmd.redirs, &in_fd) == -1)
		{
			close(pipe_fd[1]);
			exit (1);
		}
	}
}

void	execute_cmd_child_if_else(t_ast_node *ast_node, int *pipe_fd)
{
	if (ast_node->data.cmd.executable)
	{
		close(pipe_fd[1]);
		if (execve(ast_node->data.cmd.executable,
				ast_node->data.cmd.exec_argv, NULL) == -1)
			perror("execve");
		exit(1);
	}
	else
	{
		close(pipe_fd[1]);
		exit (1);
	}
}

void	execute_cmd_child_fd(int in_fd, int out_fd, int *pipe_fd)
{
	if (in_fd != -1)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 in_fd");
			close(pipe_fd[1]);
			exit (1);
		}
		close(in_fd);
	}
	if (out_fd != -1)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 out_fd");
			close(pipe_fd[1]);
			exit (1);
		}
		close(out_fd);
	}
}
