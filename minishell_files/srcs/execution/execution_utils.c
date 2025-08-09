/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:15:16 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 19:29:10 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/execution.h"

void	exec_subshell_with_redirections(t_ast_node *ast_head, int in_fd,
		int out_fd, t_shell *shell)
{
	int	status;

	if (handle_redirection_fd
		(ast_head->u_data.s_sub_shell.sub_shell_redir, &in_fd))
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
	status = ex(ast_head->u_data.s_sub_shell.subshell, -1, -1, shell);
	exit(status);
}

void	cleanup_after_left_fork_failure(int *pipe_fd)
{
	perror("fork");
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	exec_left_pipe_command(int *pipe_fd, t_ast_node *ast_head,
	int in_fd, t_shell *shell)
{
	int	status;

	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	status = ex(ast_head->u_data.s_binary_op.left, in_fd, -1, shell);
	exit(status);
}
