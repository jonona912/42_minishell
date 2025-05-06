#include "includes/execution.h"

void	exec_subshell_with_redirections(t_ast_node *ast_head, int in_fd,
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
	status = execute(ast_head->data.binary_op.left, in_fd, -1, shell);
	exit(status);
}

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
	status = execute(ast_head->data.binary_op.right, -1, out_fd, shell);
	exit(status);
}

void	close_pipe_fd(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}
