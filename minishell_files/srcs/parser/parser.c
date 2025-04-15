/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 23:51:03 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/14 19:45:53 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// need to expand the tokens before the parser.

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIRECTION_IN || type == TOKEN_REDIRECTION_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

t_token_lst	*append_redirections(t_ast_node **ast_node, t_token_lst *token_lst)
{
	t_token_type	redir_type;
	t_redir_lst		*redir_node;
	char			*temp_str;

	redir_node = NULL;
	while (token_lst && token_lst->next && is_redirection(token_lst->type)) // what if just < with nothing?????
	{
		redir_type = token_lst->type;
		token_lst = token_lst->next;
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

t_token_lst	*parse_word(t_token_lst *token_lst, t_ast_node **ast_node)
{
	// t_ast_node	*left;
	// t_ast_node	*right;
	t_token_lst	*current_token;
	int			ctr;

	ctr = 0;
	if (token_lst && (token_lst->type == TOKEN_WORD || is_redirection(token_lst->type)))
	{
		*ast_node = create_cmd_node(NODE_CMD, NULL, NULL, NULL); // handle if failure happens
		if (token_lst && is_redirection(token_lst->type))
		{
			token_lst = append_redirections(ast_node, token_lst);
			if (!token_lst)
				return (NULL); // handle error
		}
		(*ast_node)->data.cmd.executable = ft_strjoin("/bin/", token_lst->value);
		current_token = token_lst;
		while (current_token && current_token->type == TOKEN_WORD) // you can copy, double quote, single quote
		{
			ctr++;
			current_token = current_token->next;
		}
		(*ast_node)->data.cmd.exec_argv = (char **)malloc((ctr + 1) * sizeof(char *)); // if fails?
		ctr = 0;
		while (token_lst && token_lst->type == TOKEN_WORD)
		{
			(*ast_node)->data.cmd.exec_argv[ctr] = ft_strdup(token_lst->value);
			token_lst = token_lst->next;
			ctr++;
		}
		(*ast_node)->data.cmd.exec_argv[ctr] = NULL;
		if (token_lst && is_redirection(token_lst->type))
		{
			token_lst = append_redirections(ast_node, token_lst);
			if (!token_lst)
				return (NULL); // handle error
		}
		return (token_lst);
	}
	// if (token_lst && token_lst->type == TOKEN_L_PAREN)
	// {
	// 	token_lst = token_lst->next;
	// 	token_lst = parse_pipe(token_lst, ast_node);
	// 	if (token_lst->type != TOKEN_R_PAREN)
	// 		return (printf("Error: no left parent\n"), NULL); // handle error
	// }
	return (token_lst);
}

t_token_lst	*parse_subshell(t_token_lst *token_lst, t_ast_node **ast_node)
{
	// t_ast_node	*left;
	t_ast_node	*right;

	right = NULL;
	token_lst = parse_word(token_lst, ast_node);
	while (token_lst && token_lst->type == TOKEN_L_PAREN)
	{
		token_lst = token_lst->next;
		token_lst = parse_pipe(token_lst, ast_node);
		if (token_lst->type != TOKEN_R_PAREN)
			return (printf("Error: no left parent\n"), NULL);
	}
	return (token_lst);
}

t_token_lst	*parse_pipe(t_token_lst *token_lst, t_ast_node **ast_node)
{
	t_ast_node	*left;
	t_ast_node	*right;

	right = NULL;
	token_lst = parse_word(token_lst, ast_node);
	while (token_lst && token_lst->type == TOKEN_PIPE)
	{
		// node_type = token_lst->type;
		token_lst = parse_word(token_lst->next, &right);
		left = *ast_node;
		*ast_node = create_binary_op_node(NODE_PIPE, left, right);
		right = NULL;
	}
	return (token_lst);
	// if the next one is pipe, create a pipe node
}

// t_token_lst	*parse_commands(t_token_lst *token_lst, t_ast_node **ast_node)
// {
// 	t_ast_node	*left;
// 	t_ast_node	*right;
// 	t_token_lst	current_token;

// 	current_token = parse_pipe(token_lst, ast_node);
// 	while (token_lst)
// 	{

// 	}
// }
