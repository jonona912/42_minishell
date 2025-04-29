#include "../../includes/minishell.h"

int	execute_cmd_parent(pid_t fork_pid, int *pipe_fd, t_shell *shell)
{
	int		status;
	char	**new_env;
	int		count;

	close(pipe_fd[1]);
	waitpid(fork_pid, &status, 0);
	if ((status >> 8) == 96) // what is this for?
	{
		if (read(pipe_fd[0], &count, sizeof(count)) != sizeof(count))
		{
			close(pipe_fd[0]);
			return (get_exit_status(status));
		}
		new_env = (char **)malloc((count + 1) * sizeof(char *));
		if (!new_env)
		{
			close(pipe_fd[0]);
			return (get_exit_status(status));
		}
		if (!execute_cmd_parent_loop(count, new_env, pipe_fd))
			return (get_exit_status(status));
		execute_cmd_parent_free(shell, new_env);
	}
	close(pipe_fd[0]);
	return (get_exit_status(status));
}

void	execute_cmd_child_builtin(t_ast_node *ast_node,
		t_shell *shell, int *pipe_fd)
{
	int	count;

	if (ast_node->data.cmd.exec_argv
		&& builtin_check(ast_node->data.cmd.exec_argv[0]))
	{
		execute_builtin(ast_node->data.cmd.exec_argv, shell);
		count = 0;
		while (shell->env[count] != NULL)
			count++;
		if (write(pipe_fd[1], &count, sizeof(count)) == -1)
		{
			close(pipe_fd[1]);
			exit(1);
		}
		execute_cmd_child_builtin_loop(count, pipe_fd, shell);
		close(pipe_fd[1]);
		exit (0);
	}
}

int	execute_cmd_beginning(pid_t *fork_pid, int *pipe_fd, t_ast_node *ast_node)
{
	if (ast_node->data.cmd.exec_argv
		&& ft_strcmp(ast_node->data.cmd.exec_argv[0], "exit") == 0)
		exit(0);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	*fork_pid = fork();
	if (*fork_pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	return (0);
}

int	execute_cmd(t_ast_node *ast_node, int in_fd, int out_fd, t_shell *shell)
{
	pid_t	fork_pid;
	int		pipe_fd[2];

	if (ast_node->data.cmd.exec_argv && ft_strcmp(ast_node->data.cmd.exec_argv[0], "cd") == 0) // what is this for?ft_strcmp(ast_node->data.cmd.exec_argv[0], "cd")
		return (ft_cd(ast_node->data.cmd.exec_argv, shell));
	if (execute_cmd_beginning(&fork_pid, pipe_fd, ast_node))
		return (-1);
	if (fork_pid == 0)
	{
		execute_cmd_child_beginning(pipe_fd, ast_node, &in_fd);
		execute_cmd_child_fd(in_fd, out_fd, pipe_fd);
		execute_cmd_child_builtin(ast_node, shell, pipe_fd);
		execute_cmd_child_if_else(ast_node, pipe_fd);
		exit(1);
	}
	else
	{
		if (in_fd != -1)
			close(in_fd);
		if (out_fd != -1)
			close(out_fd);
		return (execute_cmd_parent(fork_pid, pipe_fd, shell));
	}
}
