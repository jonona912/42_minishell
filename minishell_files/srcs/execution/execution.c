/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:15:01 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 15:15:02 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/execution.h"

int	handle_pipe(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		left_status;
	int		right_status;

	if (preprocess_heredocs(ast_head, shell) == -1)
		return (-1);
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	left_pid = fork();
	if (left_pid == -1)
		return (cleanup_after_left_fork_failure(pipe_fd), -1);
	if (left_pid == 0)
		exec_left_pipe_command(pipe_fd, ast_head, in_fd, shell);
	right_pid = fork();
	if (right_pid == -1)
		return (cleanup_after_right_fork_failure(pipe_fd, &left_pid), -1);
	if (right_pid == 0)
		exec_right_pipe_command(pipe_fd, out_fd, shell, ast_head);
	close_pipe_fd(pipe_fd);
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	cleanup_heredocs(ast_head, shell);
	return (get_exit_status(right_status));
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
		exec_subshell_with_redirections(ast_head, in_fd, out_fd, shell);
	waitpid(fork_pid, &status, 0);
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
	return (get_exit_status(status));
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
