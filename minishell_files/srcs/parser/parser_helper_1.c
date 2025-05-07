/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:50:00 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 19:17:36 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	(void) shell;
	redir_node = NULL;
	while (token_lst && is_redirection(token_lst->type))
	{
		redir_type = token_lst->type;
		token_lst = token_lst->next;
		if (!append_redirections_if(token_lst))
			return (NULL);
		temp_str = ft_strdup(token_lst->value);
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

int	cmd_lst_loop(t_token_lst *cmd_lst, t_ast_node **ast_node)
{
	int	ctr;

	ctr = 0;
	while (cmd_lst)
	{
		(*ast_node)->u_data.s_cmd.exec_argv[ctr]
			= ft_strdup(cmd_lst->value);
		cmd_lst = cmd_lst->next;
		ctr++;
	}
	return (ctr);
}
