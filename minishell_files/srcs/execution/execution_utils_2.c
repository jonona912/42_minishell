/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:16:53 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 19:29:02 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/execution.h"

void	cleanup_after_right_fork_failure(int *pipe_fd, pid_t *left_pid)
{
	perror("fork");
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	kill(*left_pid, SIGTERM);
}

void	exec_right_pipe_command(int *pipe_fd, int out_fd,
	t_shell *shell, t_ast_node *ast_head)
{
	int	status;

	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	status = ex(ast_head->u_data.s_binary_op.right, -1, out_fd, shell);
	exit(status);
}

void	close_pipe_fd(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}
