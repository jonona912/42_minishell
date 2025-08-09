/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:18:35 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 19:25:57 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/execution.h"

void	setup_redirections_in_child(t_ast_node *ast_node, int *in_fd)
{
	if (ast_node->u_data.s_cmd.redirs
		&& is_redirection(ast_node->u_data.s_cmd.redirs->type))
	{
		if (handle_redirection_fd(ast_node->u_data.s_cmd.redirs, in_fd) == -1)
		{
			exit (1);
		}
	}
}

void	redirect_child_stdio(int in_fd, int out_fd)
{
	if (in_fd != -1)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 in_fd");
			exit (1);
		}
		close(in_fd);
	}
	if (out_fd != -1)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 out_fd");
			exit (1);
		}
		close(out_fd);
	}
}

void	exec_external_command(t_ast_node *ast_node, t_shell *shell)
{
	int	exec_return;

	exec_return = 0;
	if (ast_node->u_data.s_cmd.executable)
	{
		signal(SIGQUIT, SIG_DFL);
		exec_return = execve(ast_node->u_data.s_cmd.executable,
				ast_node->u_data.s_cmd.exec_argv, shell->env);
		if (exec_return == -1 && !ast_node->u_data.s_cmd.executable[0])
			exit (0);
		ft_putstr_fd(ast_node->u_data.s_cmd.executable, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		if (exec_return == -1
			&& opendir(ast_node->u_data.s_cmd.executable) != NULL)
			exit (126);
		else if (exec_return == -1 && ast_node->u_data.s_cmd.exec_argv[0])
			exit (127);
	}
	else
		exit (1);
}
