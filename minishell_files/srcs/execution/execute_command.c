#include "../../includes/minishell.h"

int get_exit_status(int status)
{
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		return 128 + WTERMSIG(status);
	return -1;
}

// int execute_cmd(t_ast_node *ast_node, int in_fd, int out_fd)
// {
// 	pid_t fork_pid;
// 	int status;

//     fork_pid = fork(); // *fork_pid = fork();
//     if (fork_pid == -1)
//     {
//         perror("fork");
//         return (-1);
//     }
//     if (fork_pid == 0)
//     {
//         if (ast_node->data.cmd.redirs && is_redirection(ast_node->data.cmd.redirs->type))
//         {
//             if (handle_redirection_fd(ast_node->data.cmd.redirs, &in_fd) == -1)
//                 exit (1);
//         }
//         if (in_fd != -1)
//         {
//             if (dup2(in_fd, STDIN_FILENO) == -1)
//             {
//                 perror("dup2 in_fd");
//                 exit (1);
//             }
//             close(in_fd);
//         }
//         if (out_fd != -1)
//         {
//             if (dup2(out_fd, STDOUT_FILENO) == -1)
//             {
//                 perror("dup2 out_fd");
//                 exit (1);
//             }
//             close(out_fd);
//         }
// 		if (ast_node->data.cmd.executable)
// 		{
// 			if (execve(ast_node->data.cmd.executable, ast_node->data.cmd.exec_argv, NULL) == -1)
// 			{
// 				perror("execve");
// 			}
// 			exit(1);
// 		}
// 		else
// 			exit (1);
//     }
// 	else
// 	{
// 		if (in_fd != -1)
// 			close(in_fd);
// 		if (out_fd != -1)
// 			close(out_fd);
// 		waitpid(fork_pid, &status, 0);
// 		return (get_exit_status(status));
// 	}
// }

int	execute_cmd(t_ast_node *ast_node, int in_fd, int out_fd, t_shell *shell)
{
	pid_t fork_pid;
	int status;
    int pipe_fd[2];
    // int i;

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
        if (ast_node->data.cmd.exec_argv && builtin_check(ast_node->data.cmd.exec_argv[0]))
        {
			close(pipe_fd[0]);
			// printf("DEBUG: Executing builtin with shell at %p, env at %p\n",
			// 	shell, shell ? shell->env : NULL);
			execute_builtin(ast_node->data.cmd.exec_argv, shell);
			// int count = 0;
			// while(shell->env[count] != NULL)
			// 	count++;
			// write(pipe_fd[1], &count, sizeof(count));
			// i = 0;
			// while (i < count)
			// {
			// 	int len = ft_strlen(shell->env[i]) + 1;
			// 	write(pipe_fd[1], &len, sizeof(len));
			// 	write(pipe_fd[1], shell->env[i], len);
			// 	i++;
			// }
			// // debug_env(shell);
			// // printf("\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
			close(pipe_fd[1]);
            exit (96); // need to exit with zero if successfull and other number if failed
        }
		else if (ast_node->data.cmd.executable)
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
