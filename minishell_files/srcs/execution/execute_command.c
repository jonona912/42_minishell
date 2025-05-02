#include "../../includes/minishell.h"

int	execute_cmd_parent(pid_t fork_pid)
{
	int	status;

	waitpid(fork_pid, &status, 0);
	if (WTERMSIG(status) == SIGQUIT)
		printf("Quit (core dumped)\n");
	return (get_exit_status(status));
}

void	execute_cmd_child_builtin(t_ast_node *ast_node, t_shell *shell)
{
	if (ast_node->data.cmd.exec_argv
		&& builtin_check(ast_node->data.cmd.exec_argv[0]))
	{
		execute_builtin(ast_node->data.cmd.exec_argv, shell);
		exit (0);
	}
}

int	execute_cmd_beginning(pid_t *fork_pid, t_ast_node *ast_node, t_shell *shell)
{
	int	n;

	n = 0;
	if (ast_node->data.cmd.exec_argv
		&& ft_strcmp(ast_node->data.cmd.exec_argv[0], "exit") == 0)
		n = ft_exit(ast_node->data.cmd.exec_argv);
	else if (ast_node->data.cmd.exec_argv
		&& ft_strcmp(ast_node->data.cmd.exec_argv[0], "export") == 0)
		n = ft_export(ast_node->data.cmd.exec_argv, shell);
	else if (ast_node->data.cmd.exec_argv
		&& ft_strcmp(ast_node->data.cmd.exec_argv[0], "unset") == 0)
		n = ft_unset(ast_node->data.cmd.exec_argv, shell);
	if (n != 0)
		return (n);
	*fork_pid = fork();
	if (*fork_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	return (0);
}

int	execute_cmd(t_ast_node *ast_node, int in_fd, int out_fd, t_shell *shell)
{
	pid_t	fork_pid;
	int		n;

	if (preprocess_heredocs(ast_node, shell) == -1)
		return (130);
	if (ast_node->data.cmd.exec_argv
		&& ft_strcmp(ast_node->data.cmd.exec_argv[0], "cd") == 0)
		return (ft_cd(ast_node->data.cmd.exec_argv, shell));
	n = execute_cmd_beginning(&fork_pid, ast_node, shell);
	if (n != 0)
		return (n);
	if (fork_pid == 0)
	{
		execute_cmd_child_beginning(ast_node, &in_fd);
		execute_cmd_child_fd(in_fd, out_fd);
		execute_cmd_child_builtin(ast_node, shell);
		execute_cmd_child_if_else(ast_node, shell);
		exit(1);
	}
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
	return (execute_cmd_parent(fork_pid));
}
