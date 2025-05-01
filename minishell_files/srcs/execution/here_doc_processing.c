#include "../../includes/minishell.h"

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

int	preprocess_heredocs(t_ast_node *node, t_shell *shell)
{
	t_redir_lst	*redir;
	int			fd;

	char *tmp_file;
    if (!node)
        return (-1);
    if (node->type == NODE_CMD)
	{
		redir = node->data.cmd.redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				tmp_file = append_str_and_int("/tmp/heredoc_", shell->heredoc_temp_counter);
				shell->heredoc_temp_counter++;
				fd = open(tmp_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
				if (fd == -1)
				{
					perror("open");
					exit(1);
				}
				if (handle_heredoc(redir->target, fd) == -1)
				{
					shell->last_status = 130;
					close(fd);
					return (-1);
				}
				close(fd);
				redir->type = TOKEN_REDIRECTION_IN;
				free(redir->target);
				redir->target = tmp_file;
			}
			redir = redir->next;
		}
	}
    else if (node->type == NODE_PIPE)
    {
        if (preprocess_heredocs(node->data.binary_op.left, shell) == -1)
			return (-1);
        if (preprocess_heredocs(node->data.binary_op.right, shell) == -1)
			return (-1);
    }
	// else if (node->type == SUBSHELL)
	// {
	//     preprocess_heredocs(node->data.sub_shell.subshell);
	// }
	return (0);
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
