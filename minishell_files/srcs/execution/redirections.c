/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:25:30 by opopov            #+#    #+#             */
/*   Updated: 2025/04/26 17:18:10 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_error(const char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)message, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("");
}

int	dup2_fd(int fd, int std_fd_fileno, const char *file_name)
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

int	handle_redirection_fd_if(t_redir_lst *redir_lst)
{
	int	fd;

	if (redir_lst->type == TOKEN_APPEND)
	{
		fd = open(redir_lst->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (dup2_fd(fd, STDOUT_FILENO, redir_lst->target) == -1)
			return (0);
		close(fd);
	}
	else if (redir_lst->type == TOKEN_REDIRECTION_OUT)
	{
		fd = open(redir_lst->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (dup2_fd(fd, STDOUT_FILENO, redir_lst->target) == -1)
			return (0);
		close(fd);
	}
	else if (redir_lst->type == TOKEN_REDIRECTION_IN)
	{
		fd = open(redir_lst->target, O_RDONLY);
		if (dup2_fd(fd, STDIN_FILENO, redir_lst->target) == -1)
			return (0);
		close(fd);
	}
	return (1);
}

int	handle_redirection_fd(t_redir_lst *redir_lst, int *in_fd)
{
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
		if (!handle_redirection_fd_if(redir_lst))
			return (-1);
		redir_lst = redir_lst->next;
	}
	return (0);
}
