#include "includes/parser.h"

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

int	append_redirections_if(t_token_lst *token_lst)
{
	if (!token_lst || token_lst->type == TOKEN_END
		|| !is_quote_or_word(token_lst->type))
	{
		ft_putstr_fd("minishell: Redirection without or wrong target\n", 2);
		return (0);
	}
	return (1);
}

t_token_lst	*append_redirections(t_redir_lst **node_redirs,
		t_token_lst *token_lst, t_shell *shell)
{
	t_token_type	redir_type;
	t_redir_lst		*redir_node;
	char			*temp_str;

	redir_node = NULL;
	while (token_lst && is_redirection(token_lst->type))
	{
		redir_type = token_lst->type;
		token_lst = token_lst->next;
		if (!append_redirections_if(token_lst))
			return (NULL);
		temp_str = arg_return(token_lst->value, token_lst->type, shell);
		if (!temp_str)
			return (NULL);
		redir_node = new_redir_node(redir_type, temp_str);
		if (!redir_node)
		{
			free(temp_str);
			return (NULL);
		}
		add_redir_back(node_redirs, redir_node);
		token_lst = token_lst->next;
	}
	return (token_lst);
}
