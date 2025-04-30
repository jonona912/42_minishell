#include "../../includes/minishell.h"

void	handle_pipe_right_pid_error(int *pipe_fd, pid_t *left_pid)
{
	perror("fork");
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	kill(*left_pid, SIGTERM);
}

void	handle_pipe_right_pid_child(int *pipe_fd, int out_fd,
	t_shell *shell, t_ast_node *ast_head)
{
	int	status;

	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	status = execute(ast_head->data.binary_op.right, -1, out_fd, shell);
	exit(status);
}

char *append_str_and_int(char *str, int num)
{
	char *result;
	char *num_str;

	num_str = ft_itoa(num);
	if (!num_str)
		return (NULL);
	result = ft_strjoin(str, num_str);
	free(num_str);
	return (result);
}

void preprocess_heredocs(t_ast_node *node, t_shell *shell)
{
	t_redir_lst *redir;

	char *tmp_file;
    if (!node)
        return;

    if (node->type == NODE_CMD)
    {
        redir = node->data.cmd.redirs;
        while (redir)
        {
            if (redir->type == TOKEN_HEREDOC)
            {
				tmp_file = append_str_and_int("/tmp/heredoc_", shell->heredoc_temp_counter);
				shell->heredoc_temp_counter++;
				int fd = open(tmp_file, O_RDWR | O_CREAT | O_EXCL, 0644);
                if (fd == -1)
                {
                    perror("open");
                    exit(1);
                }

				handle_heredoc(redir->target, fd);
                close(fd);
                redir->type = TOKEN_REDIRECTION_IN;
                free(redir->target);
                redir->target = ft_strdup(tmp_file);
                if (!redir->target)
                {
					return ;
                }
            }
            redir = redir->next;
        }
    }
    else if (node->type == NODE_PIPE)
    {
        preprocess_heredocs(node->data.binary_op.left, shell);
        preprocess_heredocs(node->data.binary_op.right, shell);
    }
	// else if (node->type == SUBSHELL)
	// {
	//     preprocess_heredocs(node->data.sub_shell.subshell);
	// }
}

void	cleanup_heredocs(t_ast_node *node, t_shell *shell)
{
	t_redir_lst *redir;
    if (!node)
        return;

    if (node->type == NODE_CMD)
    {
        redir = node->data.cmd.redirs;
        while (redir)
        {
            if (redir->type == TOKEN_REDIRECTION_IN &&
                ft_strstr(redir->target, "/tmp/heredoc_") == redir->target)
            {
                unlink(redir->target);
				shell->heredoc_temp_counter--;

            }
            redir = redir->next;
        }
    }
    else if (node->type == NODE_PIPE)
    {
        cleanup_heredocs(node->data.binary_op.left, shell);
        cleanup_heredocs(node->data.binary_op.right, shell);
    }
    // else if (node->type == SUBSHELL)
    // {
    //     cleanup_heredocs(node->data.sub_shell.subshell);
    // }
}

// solve this later
int	handle_pipe(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		left_status;
	int		right_status;

	preprocess_heredocs(ast_head, shell);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	// search for here_doc, if found then run it and make temporary file
	left_pid = fork();
	if (left_pid == -1)
		return (handle_pipe_left_pid_error(pipe_fd), -1);
	if (left_pid == 0)
		handle_pipe_left_pid_child(pipe_fd, ast_head, in_fd, shell);
	// search for here_doc, if found then run it and make temporary file
	right_pid = fork();
	if (right_pid == -1)
		return (handle_pipe_right_pid_error(pipe_fd, &left_pid), -1);
	if (right_pid == 0)
		handle_pipe_right_pid_child(pipe_fd, out_fd, shell, ast_head);
	close_pipe_fd(pipe_fd);
	// delete temporary file
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	cleanup_heredocs(ast_head, shell);
	return (right_status);
}

int	handle_subshell(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell)
{
	pid_t	fork_pid;
	int		status;

	fork_pid = fork();
	if (fork_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (fork_pid == 0)
		handle_subshell_if(ast_head, in_fd, out_fd, shell);
	waitpid(fork_pid, &status, 0);
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
	return (status);
}

int	execute(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell)
{
	int	status;

	if (ast_head->type == NODE_CMD)
		return (execute_cmd(ast_head, in_fd, out_fd, shell));
	else if (ast_head->type == NODE_SUBSHELL)
		return (handle_subshell(ast_head, in_fd, out_fd, shell));
	else if (ast_head->type == NODE_PIPE)
		return (handle_pipe(ast_head, in_fd, out_fd, shell));
	else if (ast_head->type == NODE_AND)
	{
		status = execute(ast_head->data.binary_op.left, in_fd, out_fd, shell);
		if (!status)
			status = execute(ast_head->data.binary_op.right,
					in_fd, out_fd, shell);
		return (status);
	}
	else if (ast_head->type == NODE_OR)
	{
		status = execute(ast_head->data.binary_op.left, in_fd, out_fd, shell);
		if (status)
			status = execute(ast_head->data.binary_op.right,
					in_fd, out_fd, shell);
		return (status);
	}
	return (-1);
}
