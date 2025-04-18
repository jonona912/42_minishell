#include "../../includes/minishell.h"

void	print_error(const char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)message, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("");
}

int dup2_fd(int fd, int std_fd_fileno, const char *file_name)
{
	if (fd == -1)
	{
		print_error(file_name);
		return (-1);
	}
	if (dup2(fd, std_fd_fileno) == -1)
	{
		print_error(file_name);
		return (-1);
	}
	return (0);
}

int	ms_strcmp_until(char *s1, char *s2, char c)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (*(s1 + i) && *(s2 + i) && *(s1 + i) != c && *(s1 + i) == *(s2 + i))
		i++;
	return (*(s1 + i) - *(s2 + i));
}

int	handle_heredoc(char *end_delimitor, int in_fd) // here if single quotes, then you can strip it and // handle cat << ''
{
	char	*input;

	if (!end_delimitor)
		return (-1);
	while(1)
	{
		input = readline("\033[0;35mheredoc>\033[0m");
		if (ms_strcmp_until(end_delimitor, input, '\n') == 0)
		{
			free(input);
			break ;
		}
		ft_putstr_fd(input, in_fd);
		ft_putstr_fd("\n", in_fd);
		free(input);
	}
	return (1);
}

int	run_heredoc(char *end_delimitor, int *in_fd)//, int *out_fd)
{
	int		pipe_fd[2];
	int		is_interprete;
	char	*temp;

	temp = end_delimitor;
	is_interprete = 1;
	if (end_delimitor == NULL)
		return (-1);
	if (*end_delimitor == '\"' || *end_delimitor == '\'')
	{
		end_delimitor = ft_strtrim(end_delimitor, "\"\'");
		if (!end_delimitor)
			return (-1);
		is_interprete = 0;
	}
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (handle_heredoc(end_delimitor, pipe_fd[1]) == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	close(pipe_fd[1]);
	*in_fd = pipe_fd[0];
	if (!is_interprete)
		free(end_delimitor);
	return (0);
}

int handle_redirection_fd(t_redir_lst *redir_lst, int *in_fd)//, int *out_fd)
{
	int		fd;
	t_redir_lst	*traverse_redir_lst;

	traverse_redir_lst = redir_lst;
	while (traverse_redir_lst)
	{
		if (traverse_redir_lst->type == TOKEN_HEREDOC)
		{
			if (run_heredoc(traverse_redir_lst->target, in_fd) == -1)
				return (-1);
			break ;
		}
		traverse_redir_lst = traverse_redir_lst->next;
	}
	while (redir_lst)
	{
		if (redir_lst->type == TOKEN_APPEND)
		{
			fd = open(redir_lst->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (dup2_fd(fd, STDOUT_FILENO, redir_lst->target) == -1)
				return (-1);
			close(fd);
		}
		else if (redir_lst->type == TOKEN_REDIRECTION_OUT)
		{
			fd = open(redir_lst->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (dup2_fd(fd, STDOUT_FILENO, redir_lst->target) == -1)
				return (-1);
			close(fd);
		}
		else if (redir_lst->type == TOKEN_REDIRECTION_IN)
		{
			fd = open(redir_lst->target, O_RDONLY);
			if (dup2_fd(fd, STDIN_FILENO, redir_lst->target) == -1)
				return (-1);
			close(fd);
		}
		redir_lst = redir_lst->next;
	}
	return (0);
}

#include <errno.h>

int	execute_cmd(t_ast_node *ast_node, int in_fd, int out_fd, t_shell *shell)//, pid_t *pids, int *pid_count)
{
	int		res;
	pid_t	fork_pid;
	// int		status;

	fork_pid = fork();
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
		if (ast_node->data.cmd.exec_argv && builtin_check(ast_node->data.cmd.exec_argv[0]))
		{
			res = execute_builtin(ast_node->data.cmd.exec_argv, shell);
			exit (res);
		}
		else if (ast_node->data.cmd.executable)
		{
			if (execve(ast_node->data.cmd.executable, ast_node->data.cmd.exec_argv, NULL) == -1)
			{
				perror("execve");
			}
			exit(127); // not working
		}
		else
			exit (3);
	}
	else
	{
		waitpid(fork_pid, &shell->last_status, 0);
		return (1);
	}
}

int	execute(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell) //, pid_t *pids, int *pid_count)
{
	int pipe_fd[2];
	int	status;

	if (ast_head->type == NODE_CMD)
	{
		status = execute_cmd(ast_head, in_fd, out_fd, shell);
		return (status);
	}
	else if (ast_head->type == NODE_PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (-1);
		}
		status = execute(ast_head->data.binary_op.left, in_fd, pipe_fd[1], shell);
		if (status != -1)
			status = execute(ast_head->data.binary_op.right, pipe_fd[0], out_fd, shell);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (status);
	}
	else if (ast_head->type == NODE_AND)
	{
		status = execute(ast_head->data.binary_op.left, -1, -1, shell); // , pids, pid_count
		if (status)
			status = execute(ast_head->data.binary_op.right, -1, -1, shell); // , pids, pid_count
		return (status);
	}
	else if (ast_head->type == NODE_OR)
	{
		status = execute(ast_head->data.binary_op.left, -1, -1, shell); // , pids, pid_count
		if (!status)
			status = execute(ast_head->data.binary_op.right, -1, -1, shell); // , pids, pid_count
		return (status);
	}
	return (-1);
}

// int	run_pipeline(t_ast_node *ast_head, t_shell *shell) // run pipeline can return the exit status of the last command
// {
// 	// pid_t pids[10];
// 	// int pid_count = 0;
// 	return (execute(ast_head, -1, -1, shell));//, pids, &pid_count);
// 	// for (int i = 0; i < pid_count; i++)
// 	// 	waitpid(pids[i], NULL, 0);
// 	// return (0); // success but change it to return the exit status of the last command
// }


// if NODE_AND -- exec right if left success
// if NODE_OR -- exec right if left fails
