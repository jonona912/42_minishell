/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:26:58 by opopov            #+#    #+#             */
/*   Updated: 2025/04/28 10:12:01 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_pipe_right_pid_error(int *pipe_fd, pid_t *left_pid)
{
	perror("fork");
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	kill(*left_pid, SIGTERM);
}

void	handle_pipe_right_pid_child(int *pipe_fd, int out_fd,
	t_shell *shell, t_ast_node *ast_head)
{
	int	status;

	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	status = execute(ast_head->data.binary_op.right, -1, out_fd, shell);
	exit(status);
}

// solve this later
int	handle_pipe(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		left_status;
	int		right_status;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	left_pid = fork();
	if (left_pid == -1)
		return (handle_pipe_left_pid_error(pipe_fd), -1);
	if (left_pid == 0)
		handle_pipe_left_pid_child(pipe_fd, ast_head, in_fd, shell);
	right_pid = fork();
	if (right_pid == -1)
		return (handle_pipe_right_pid_error(pipe_fd, &left_pid), -1);
	if (right_pid == 0)
		handle_pipe_right_pid_child(pipe_fd, out_fd, shell, ast_head);
	close_pipe_fd(pipe_fd);
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	return (right_status);
}

int	handle_subshell(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell)
{
	pid_t	fork_pid;
	int		status;

	fork_pid = fork();
	if (fork_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (fork_pid == 0)
		handle_subshell_if(ast_head, in_fd, out_fd, shell);
	waitpid(fork_pid, &status, 0);
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
	return (status);
}

int	execute(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell)
{
	int	status;

	if (ast_head->type == NODE_CMD)
		return (execute_cmd(ast_head, in_fd, out_fd, shell));
	else if (ast_head->type == NODE_SUBSHELL)
		return (handle_subshell(ast_head, in_fd, out_fd, shell));
	else if (ast_head->type == NODE_PIPE)
		return (handle_pipe(ast_head, in_fd, out_fd, shell));
	else if (ast_head->type == NODE_AND)
	{
		status = execute(ast_head->data.binary_op.left, in_fd, out_fd, shell);
		if (!status)
			status = execute(ast_head->data.binary_op.right,
					in_fd, out_fd, shell);
		return (status);
	}
	else if (ast_head->type == NODE_OR)
	{
		status = execute(ast_head->data.binary_op.left, in_fd, out_fd, shell);
		if (status)
			status = execute(ast_head->data.binary_op.right,
					in_fd, out_fd, shell);
		return (status);
	}
	return (-1);
}
