#include "../../includes/minishell.h"

int get_exit_status(int status)
{
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		return 128 + WTERMSIG(status);
	return -1;
}

int execute_cmd(t_ast_node *ast_node, int in_fd, int out_fd)
{
	pid_t fork_pid;
	int status;

    fork_pid = fork(); // *fork_pid = fork();
    if (fork_pid == -1)
    {
        perror("fork");
        return (-1);
    }
    if (fork_pid == 0)
    {
        if (ast_node->data.cmd.redirs && is_redirection(ast_node->data.cmd.redirs->type))
        {
            if (handle_redirection_fd(ast_node->data.cmd.redirs, &in_fd) == -1)
                exit (1);
        }
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
		if (ast_node->data.cmd.executable)
		{
			if (execve(ast_node->data.cmd.executable, ast_node->data.cmd.exec_argv, NULL) == -1)
			{
				perror("execve");
			}
			exit(1);
		}
		else
			exit (1);
    }
	else
	{
		if (in_fd != -1)
			close(in_fd);
		if (out_fd != -1)
			close(out_fd);
		waitpid(fork_pid, &status, 0);
		return (get_exit_status(status));
	}
}
