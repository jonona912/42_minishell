/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:30:37 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 18:31:21 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//execution
void	exec_subshell_with_redirections(
			t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell);
int		get_exit_status(int status);
void	setup_redirections_in_child(t_ast_node *ast_node, int *in_fd);
void	exec_external_command(t_ast_node *ast_node, t_shell *shell);
void	redirect_child_stdio(int in_fd, int out_fd);
void	close_pipe_fd(int *pipe_fd);
void	cleanup_after_left_fork_failure(int *pipe_fd);
void	exec_left_pipe_command(int *pipe_fd,
			t_ast_node *ast_head, int in_fd, t_shell *shell);
void	cleanup_after_right_fork_failure(int *pipe_fd, pid_t *left_pid);
void	exec_right_pipe_command(int *pipe_fd,
			int out_fd, t_shell *shell, t_ast_node *ast_head);
int		ms_strcmp_until(char *s1, char *s2, char c);
int		handle_heredoc_eof(struct sigaction *old_sa);
void	handle_heredoc_input(char *input, int in_fd);

// built_in_commands
int		ft_exit(char **argv);
void	ft_env(t_shell *shell);
void	ft_pwd(void);
