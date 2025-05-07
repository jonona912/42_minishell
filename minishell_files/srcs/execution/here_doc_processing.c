/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_processing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:14:54 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 15:14:55 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/execution.h"

char	*append_str_and_int(char *str, int num)
{
	char	*result;
	char	*num_str;

	num_str = ft_itoa(num);
	if (!num_str)
		return (NULL);
	result = ft_strjoin(str, num_str);
	free(num_str);
	return (result);
}

int	process_heredoc_helper(t_redir_lst *redir,
		t_shell *shell, int fd, char *tmp_file)
{
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			tmp_file = append_str_and_int("/tmp/heredoc_", shell->heredoc_ctr);
			shell->heredoc_ctr++;
			fd = open(tmp_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("open");
				exit(1);
			}
			if (handle_heredoc(redir->target, fd, &tmp_file) == -1)
			{
				shell->last_status = 130;
				return (close(fd), -1);
			}
			close(fd);
			redir->type = TOKEN_REDIRECTION_IN;
			free(redir->target);
			redir->target = tmp_file;
		}
		redir = redir->next;
	}
	return (0);
}

int	preprocess_heredocs(t_ast_node *node, t_shell *shell)
{
	int			fd;
	char		*tmp_file;

	fd = 0;
	tmp_file = NULL;
	if (!node)
		return (-1);
	if (node->type == NODE_CMD)
	{
		return (process_heredoc_helper(node->data.cmd.redirs,
				shell, fd, tmp_file));
	}
	else if (node->type == NODE_PIPE)
	{
		if (preprocess_heredocs(node->data.binary_op.left, shell) == -1)
			return (-1);
		if (preprocess_heredocs(node->data.binary_op.right, shell) == -1)
			return (-1);
	}
	return (0);
}

void	cleanup_heredocs(t_ast_node *node, t_shell *shell)
{
	t_redir_lst	*redir;

	if (!node)
		return ;
	if (node->type == NODE_CMD)
	{
		redir = node->data.cmd.redirs;
		while (redir)
		{
			if (redir->type == TOKEN_REDIRECTION_IN
				&& ft_strstr(redir->target,
					"/tmp/heredoc_") == redir->target)
			{
				unlink(redir->target);
				shell->heredoc_ctr--;
			}
			redir = redir->next;
		}
	}
	else if (node->type == NODE_PIPE)
	{
		cleanup_heredocs(node->data.binary_op.left, shell);
		cleanup_heredocs(node->data.binary_op.right, shell);
	}
}
