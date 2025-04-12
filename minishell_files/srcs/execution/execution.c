/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:54:11 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/12 23:31:27 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// pipefd: An array of two integers. After a successful call:
// pipefd[0] is the file descriptor for the read end of the pipe.
// pipefd[1] is the file descriptor for the write end of the pipe.

// int execute_cmd(t_ast_node *ast_node, int pipe_direction, int pipe_fd[2])
// {
// 	pid_t fork_pid = fork();
// 	if (fork_pid == 0)
// 	{
// 		if (pipe_direction == 1)
// 		{
// 			close(pipe_fd[0]);
// 			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
// 			{
// 				perror("dup2");
// 				return (-1);
// 			}
// 			close(pipe_fd[1]);
// 		}
// 		if (pipe_direction == 2)
// 		{
// 			if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
// 			{
// 				perror("dup2");
// 				return (-1);
// 			}
// 			close(pipe_fd[0]);
// 		}
// 		if (execve(ast_node->data.cmd.executable, ast_node->data.cmd.exec_argv, NULL) == -1)
// 			return (0);
// 	}
// 	wait(NULL);
// 	// printf("waited for %s\n", ast_node->data.cmd.executable);
// 	return (1);
// }

// int	execute(t_ast_node *ast_head, int pipe_direction, int pipe_fd[2]) // pipe = 0 no_pipe, 1 pipe out, 2 pipe_in
// {
// 	int	left;
// 	int	right;
// 	// int	pipe_fd[2];

// 	if (ast_head->type == NODE_PIPE)
// 	{
// 		if (pipe(ast_head->data.binary_op.pipe_fd) == -1)
// 		{
// 			perror("pipe:");
// 			return (-1);
// 		}
// 		// if node is piped then create a pipe
// 		left = execute(ast_head->data.binary_op.left, 1, ast_head->data.binary_op.pipe_fd);
// 		close(ast_head->data.binary_op.pipe_fd[1]);
// 		right = execute(ast_head->data.binary_op.right, 2, ast_head->data.binary_op.pipe_fd);
// 		close(ast_head->data.binary_op.pipe_fd[0]);
// 	}
// 	else
// 	{
// 		return (execute_cmd(ast_head, pipe_direction, pipe_fd));
// 	}
// 	return (1);
// }




// pseudocode

// create a fork




#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

// Execute a command with specified input and output file descriptors
int execute_cmd(t_ast_node *ast_node, int in_fd, int out_fd, pid_t *pids, int *pid_count)
{
    pid_t fork_pid = fork();
    if (fork_pid == -1)
    {
        perror("fork");
        return (-1);
    }
    if (fork_pid == 0) // Child process
    {
        // Redirect input if specified
        if (in_fd != -1)
        {
            if (dup2(in_fd, STDIN_FILENO) == -1)
            {
                perror("dup2 in_fd");
                exit(1);
            }
            close(in_fd);
        }
        // Redirect output if specified
        if (out_fd != -1)
        {
            if (dup2(out_fd, STDOUT_FILENO) == -1)
            {
                perror("dup2 out_fd");
                exit(1);
            }
            close(out_fd);
        }
        if (ast_node->data.cmd.redirs && ast_node->data.cmd.redirs->type == TOKEN_REDIRECTION_IN)
        {
            int fd = open(ast_node->data.cmd.redirs->target, O_RDONLY);
            if (fd == -1)
            {
                perror("open redirection in");
                exit(1);
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2 redirection in");
                exit(1);
            }
            close(fd);
        }
        if (ast_node->data.cmd.redirs && ast_node->data.cmd.redirs->type == TOKEN_REDIRECTION_OUT)
        {
            int fd = open(ast_node->data.cmd.redirs->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("open redirection out");
                exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2 redirection out");
                exit(1);
            }
            close(fd);
        }
        // Execute the command
        if (execve(ast_node->data.cmd.executable, ast_node->data.cmd.exec_argv, NULL) == -1)
        {
            perror("execve");
            exit(1);
        }
    }
    // Parent process: store PID and close fds
    pids[*pid_count] = fork_pid;
    (*pid_count)++;
    if (in_fd != -1)
        close(in_fd); // not double closing?
    if (out_fd != -1)
        close(out_fd);
    return (1);
}
// // pipefd: An array of two integers. After a successful call:
// // pipefd[0] is the file descriptor for the read end of the pipe.
// // pipefd[1] is the file descriptor for the write end of the pipe.
// Execute the AST, collecting PIDs
int execute(t_ast_node *ast_head, int in_fd, int out_fd, pid_t *pids, int *pid_count)
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
        // Execute left child: input from in_fd, output to pipe_fd[1]
        if (execute(ast_head->data.binary_op.left, in_fd, pipe_fd[1], pids, pid_count) == -1)
            return (-1);
        // Execute right child: input from pipe_fd[0], output to out_fd
        if (execute(ast_head->data.binary_op.right, pipe_fd[0], out_fd, pids, pid_count) == -1)
            return (-1);
        // Close pipe fds in parent
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return (1);
    }
    return (-1); // Invalid node type
}

// Top-level function to run the pipeline
void run_pipeline(t_ast_node *ast_head)
{
    pid_t pids[10]; // Adjust size based on max expected commands
    int pid_count = 0;
    execute(ast_head, -1, -1, pids, &pid_count);
    // Wait for all child processes
    for (int i = 0; i < pid_count; i++)
        waitpid(pids[i], NULL, 0);
}
