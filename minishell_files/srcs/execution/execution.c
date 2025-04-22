#include "../../includes/minishell.h"

int	handle_pipe(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell)
{
	int pipe_fd[2];
	pid_t left_pid;
	pid_t right_pid;
	int status;
	int left_status;
	int	right_status;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return -1;
	}
	left_pid = fork();
	if (left_pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return -1;
	}
	if (left_pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		status = execute(ast_head->data.binary_op.left, in_fd, -1, shell);
		exit(status == -1 ? 1 : status);
	}
	right_pid = fork();
	if (right_pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		kill(left_pid, SIGTERM);
		return -1;
	}
	if (right_pid == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		status = execute(ast_head->data.binary_op.right, -1, out_fd, shell);
		exit(status); // what is this??
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	return (right_status);
}

int handle_subshell(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell)
{
	pid_t fork_pid;
	int status;

	fork_pid = fork();
	if (fork_pid == -1)
	{
		perror("fork");
		return -1;
	}
	if (fork_pid == 0)
	{
		if (handle_redirection_fd(ast_head->data.sub_shell.sub_shell_redir, &in_fd))
			exit(1);
		if (in_fd != -1) {
			dup2(in_fd, STDIN_FILENO);
			close(in_fd);
		}
		if (out_fd != -1) {
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
		}
		status = execute(ast_head->data.sub_shell.subshell, -1, -1, shell);
		exit(status);
	}
	waitpid(fork_pid, &status, 0);
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
	return (status);
}

// int	execute(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell)
// {
// 	int status;

// 	if (ast_head->type == NODE_CMD) // returns command function
// 	{
// 		return (execute_cmd(ast_head, in_fd, out_fd, shell));
// 	}
// 	else if (ast_head->type == NODE_SUBSHELL)
// 	{
// 		return (handle_subshell(ast_head, in_fd, out_fd));
// 	}
// 	else if (ast_head->type == NODE_PIPE)
// 	{
// 		return (handle_pipe(ast_head, in_fd, out_fd)); // returns cmd exit status (processed)
// 	}
// 	else if (ast_head->type == NODE_AND) // node or and and get status through returning
// 	{
// 		status = execute(ast_head->data.binary_op.left, in_fd, out_fd);
// 		if (!status) // made changes here to reflect exit status
// 			status = execute(ast_head->data.binary_op.right, in_fd, out_fd);
// 		return (status);
// 	}
// 	else if (ast_head->type == NODE_OR)
// 	{
// 		status = execute(ast_head->data.binary_op.left, in_fd, out_fd);
// 		if (status)
// 			status = execute(ast_head->data.binary_op.right, in_fd, out_fd);
// 		return (status);
// 	}
// 	return -1;
// }

int	execute(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell)
{
	int status;

	if (ast_head->type == NODE_CMD) // returns command function
	{
		return (execute_cmd(ast_head, in_fd, out_fd, shell));
	}
	else if (ast_head->type == NODE_SUBSHELL)
	{
		return (handle_subshell(ast_head, in_fd, out_fd, shell));
	}
	else if (ast_head->type == NODE_PIPE)
	{
		return (handle_pipe(ast_head, in_fd, out_fd, shell)); // returns cmd exit status (processed)
	}
	else if (ast_head->type == NODE_AND) // node or and and get status through returning
	{
		status = execute(ast_head->data.binary_op.left, in_fd, out_fd, shell);
		if (!status) // made changes here to reflect exit status
			status = execute(ast_head->data.binary_op.right, in_fd, out_fd, shell);
		return (status);
	}
	else if (ast_head->type == NODE_OR)
	{
		status = execute(ast_head->data.binary_op.left, in_fd, out_fd, shell);
		if (status)
			status = execute(ast_head->data.binary_op.right, in_fd, out_fd, shell);
		return (status);
	}
	return -1;
}
