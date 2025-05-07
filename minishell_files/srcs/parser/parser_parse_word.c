/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parse_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:49:47 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 19:27:52 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/parser.h"

t_token_lst	*handle_parentheses(t_token_lst *token_lst,
		t_ast_node **ast_node, t_shell *shell)
{
	token_lst = token_lst->next;
	if (token_lst && token_lst->type == TOKEN_R_PAREN)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		return (NULL);
	}
	token_lst = parse_or(token_lst, ast_node, shell);
	if (!token_lst)
	{
		return (NULL);
	}
	if (token_lst && token_lst->type != TOKEN_R_PAREN)
		return (ft_putstr_fd("Error: no right parenthesis\n", 2), NULL);
	token_lst = token_lst->next;
	if (token_lst && (token_lst->type == TOKEN_AND
			|| token_lst->type == TOKEN_OR
			|| token_lst->type == TOKEN_PIPE
			|| token_lst->type == TOKEN_END))
	{
		return (token_lst);
	}
	return (token_lst);
}

t_token_lst	*append_redirections_if_any(t_token_lst *token_lst,
		t_redir_lst **node_redirs, t_shell *shell)
{
	if (token_lst && is_redirection(token_lst->type))
	{
		token_lst = append_redirections(node_redirs, token_lst, shell);
		if (!token_lst)
		{
			return (ft_putstr_fd
				("minishell: syntax error near unexpected token\n", 2), NULL);
		}
	}
	return (token_lst);
}

int	count_token_words(t_token_lst *token_lst)
{
	int	count;

	count = 0;
	while (token_lst && token_lst->type == TOKEN_WORD)
	{
		count++;
		token_lst = token_lst->next;
	}
	return (count);
}

t_token_lst	*parse_word(t_token_lst *token_lst,
						t_ast_node **ast_node,
						t_shell *shell)
{
	if (token_lst && (token_lst->type == TOKEN_WORD
			|| is_redirection(token_lst->type)
			|| token_lst->type == TOKEN_ENV_VAR
			|| token_lst->type == TOKEN_D_QUOTE
			|| token_lst->type == TOKEN_S_QUOTE))
	{
		token_lst = parse_cmd(token_lst, ast_node, shell);
		return (token_lst);
	}
	if (token_lst && token_lst->type == TOKEN_L_PAREN)
	{
		token_lst = handle_parentheses(token_lst, ast_node, shell);
		if (!token_lst)
			return (NULL);
		*ast_node = create_subshell_node(NODE_SUBSHELL, *ast_node, NULL);
		token_lst = append_redirections_if_any
			(token_lst, &(*ast_node)->u_data.s_sub_shell.sub_shell_redir,
				shell);
		if (!token_lst)
			return (NULL);
	}
	return (token_lst);
}
