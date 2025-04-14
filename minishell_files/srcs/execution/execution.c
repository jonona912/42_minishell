/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:54:11 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/13 22:56:44 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int dup2_fd(int fd, int std_fd_fileno)
{
	if (fd == -1)
	{
		perror("open redirection in");
		return (-1);
	}
	if (dup2(fd, std_fd_fileno) == -1)
	{
		perror("dup2 redirection in");
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

int run_heredoc(char *end_delimitor, int *in_fd, int *out_fd)
{
    int pipe_fd[2];
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
	if (*out_fd == -20)
		*out_fd = pipe_fd[0];
    close(pipe_fd[1]);
    *in_fd = pipe_fd[0];
    return (0);
}

int handle_redirection_fd(t_redir_lst *redir_lst, int *in_fd, int *out_fd)
{
	int		fd;
	t_redir_lst	*traverse_redir_lst;


	traverse_redir_lst = redir_lst;
	while (traverse_redir_lst)
	{
		if (traverse_redir_lst->type == TOKEN_HEREDOC)
		{
			if (run_heredoc(traverse_redir_lst->target, in_fd, out_fd) == -1)
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
			if (dup2_fd(fd, STDOUT_FILENO) == -1)
				return (-1);
			close(fd);
		}
		else if (redir_lst->type == TOKEN_REDIRECTION_OUT)
		{
			fd = open(redir_lst->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (dup2_fd(fd, STDOUT_FILENO) == -1)
				return (-1);
			close(fd);
		}
		else if (redir_lst->type == TOKEN_REDIRECTION_IN)
		{
			fd = open(redir_lst->target, O_RDONLY);
			if (dup2_fd(fd, STDIN_FILENO) == -1)
				return (-1);
			close(fd);
		}
		redir_lst = redir_lst->next;
	}
	return (0);
}

int execute_cmd(t_ast_node *ast_node, int in_fd, int out_fd, pid_t *pids, int *pid_count)
{
    pid_t fork_pid = fork();
    if (fork_pid == -1)
    {
        perror("fork");
        return (-1);
    }
    if (fork_pid == 0)
    {
        if (ast_node->data.cmd.redirs && is_redirection(ast_node->data.cmd.redirs->type))
        {
            if (handle_redirection_fd(ast_node->data.cmd.redirs, &in_fd, &out_fd) == -1)
                exit(1);
        }
        if (in_fd != -1)
        {
            if (dup2(in_fd, STDIN_FILENO) == -1)
            {
                perror("dup2 in_fd");
                exit(1);
            }
            close(in_fd);
        }
        if (out_fd != -1)
        {
            if (dup2(out_fd, STDOUT_FILENO) == -1)
            {
                perror("dup2 out_fd");
                exit(1);
            }
            close(out_fd);
        }
		if (ast_node->data.cmd.executable)
		{
			if (execve(ast_node->data.cmd.executable, ast_node->data.cmd.exec_argv, NULL) == -1)
			{
				perror("execve");
				exit(1);
			}
		}
		else
			exit(0);
    }
    pids[*pid_count] = fork_pid;
    (*pid_count)++;
    if (in_fd != -1)
        close(in_fd);
    if (out_fd != -1)
        close(out_fd);
    return (1);
}

// // pipefd: An array of two integers. After a successful call:
// // pipefd[0] is the file descriptor for the read end of the pipe.
// // pipefd[1] is the file descriptor for the write end of the pipe.
// Execute the AST, collecting PIDs
int	execute(t_ast_node *ast_head, int in_fd, int out_fd, pid_t *pids, int *pid_count)
{
	if (ast_head->type == NODE_CMD)
	{
		return execute_cmd(ast_head, in_fd, out_fd, pids, pid_count);
	}
	else if (ast_head->type == NODE_PIPE)
	{
		int pipe_fd[2];
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (-1);
		}
		if (execute(ast_head->data.binary_op.left, in_fd, pipe_fd[1], pids, pid_count) == -1)
			return (-1);
		if (execute(ast_head->data.binary_op.right, pipe_fd[0], out_fd, pids, pid_count) == -1)
			return (-1);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	return (-1);
}

void run_pipeline(t_ast_node *ast_head)
{
	pid_t pids[10];
	int pid_count = 0;
	execute(ast_head, -1, -1, pids, &pid_count);
	for (int i = 0; i < pid_count; i++)
		waitpid(pids[i], NULL, 0);
}
