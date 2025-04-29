#include "../../includes/minishell.h"

int	builtin_check(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "env") == 0);
}

int	execute_builtin(char **argv, t_shell *shell) // handle $P
{
	// printf("argv[0]: %s\n", argv[0]);
	if (ft_strcmp(argv[0], "echo") == 0)
		ft_echo(argv, shell);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		ft_pwd(shell);
	else if (ft_strcmp(argv[0], "export") == 0)
		return (ft_export(argv, shell));
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (ft_unset(argv, shell));
	else if (ft_strcmp(argv[0], "env") == 0)
		ft_env(shell);
	// else if (ft_strcmp(argv[0], "exit") == 0)
	// 	exit(1);
	return (0);
}

void	handle_pipe_left_pid_error(int *pipe_fd)
{
	perror("fork");
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	handle_pipe_left_pid_child(int *pipe_fd, t_ast_node *ast_head,
	int in_fd, t_shell *shell)
{
	int	status;

	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	status = execute(ast_head->data.binary_op.left, in_fd, -1, shell);
	exit(status);
}
