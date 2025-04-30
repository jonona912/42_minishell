#include "../../includes/minishell.h"

void	execute_cmd_child_builtin_loop(int count, int *pipe_fd, t_shell *shell)
{
	int	i;
	int	len;

	i = 0;
	while (i < count)
	{
		len = ft_strlen(shell->env[i]) + 1;
		if (write(pipe_fd[1], &len, sizeof(len)) == -1
			|| write(pipe_fd[1], shell->env[i], len) == -1)
		{
			close(pipe_fd[1]);
			exit(1);
		}
		i++;
	}
}

void	execute_cmd_child_beginning(t_ast_node *ast_node, int *in_fd)
{
	if (ast_node->data.cmd.redirs && is_redirection(ast_node->data.cmd.redirs->type))
	{
		if (handle_redirection_fd(ast_node->data.cmd.redirs, in_fd) == -1)
		{
			exit (2); // problem here for exit status echo hi | echo bye > invalid_file
		}
	}
}

void	execute_cmd_child_if_else(t_ast_node *ast_node)
{
	int	exec_return;

	exec_return = 0;
	if (ast_node->data.cmd.executable)
	{
		exec_return = execve(ast_node->data.cmd.executable, ast_node->data.cmd.exec_argv, NULL);
		if (exec_return == -1 && !ast_node->data.cmd.executable[0])
		{
			exit (0);
		}
		ft_putstr_fd(ast_node->data.cmd.executable, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		if (exec_return == -1 && opendir(ast_node->data.cmd.executable) != NULL)
		{
			exit (126);
		}
		else if (exec_return == -1 && ast_node->data.cmd.exec_argv[0])
		{
			exit (127);
		}
	}
	else
	{
		exit (1);
	}
}

void	execute_cmd_child_fd(int in_fd, int out_fd)
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

void	close_pipe_fd(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}
