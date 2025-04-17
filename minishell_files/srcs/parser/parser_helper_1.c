#include "../../includes/minishell.h"

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIRECTION_IN || type == TOKEN_REDIRECTION_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int	is_quote_or_word(t_token_type type)
{
	return (type == TOKEN_S_QUOTE || type == TOKEN_D_QUOTE
		|| type == TOKEN_WORD);
}

t_token_lst	*append_redirections(t_ast_node **ast_node, t_token_lst *token_lst) // redirections can store s_quotes and d_quotes (tokens)
{
	t_token_type	redir_type;
	t_redir_lst		*redir_node;
	char			*temp_str;

	redir_node = NULL;
	while (token_lst && is_redirection(token_lst->type)) // what if just < with nothing?????
	{
		redir_type = token_lst->type;
		token_lst = token_lst->next;
		if (!token_lst || token_lst->type == TOKEN_END || !is_quote_or_word(token_lst->type))
		{
			ft_putstr_fd("\033[31mminishell: Redirection without or wrong target\033[0m\n", 2); // print in red
			return NULL; // Error: Redirection without target cat <
		}
		temp_str = ft_strdup(token_lst->value);
		if (!temp_str)
			return (NULL); // handle error
		redir_node = new_redir_node(redir_type, temp_str);
		if (!redir_node)
		{
			free(temp_str);
			return (NULL); // handle error
		}

		add_redir_back(&(*ast_node)->data.cmd.redirs, redir_node);
		token_lst = token_lst->next;
	}
	return (token_lst);
}





