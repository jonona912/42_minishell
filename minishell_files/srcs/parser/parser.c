#include "../../includes/minishell.h"

// need to expand the tokens before the parser.

t_token_lst	*parse_pipe(t_token_lst *token_lst, t_ast_node **ast_node)
{
	t_ast_node	*left;
	t_ast_node	*right;

	right = NULL;
	token_lst = parse_word(token_lst, ast_node);
	while (token_lst && token_lst->type == TOKEN_PIPE)
	{
		token_lst = parse_word(token_lst->next, &right);
		left = *ast_node;
		*ast_node = create_binary_op_node(NODE_PIPE, left, right);
		right = NULL;
	}
	return (token_lst);
}

t_token_lst	*parse_and(t_token_lst *token_lst, t_ast_node **ast_node)
{
	t_ast_node	*left;
	t_ast_node	*right;

	right = NULL;
	token_lst = parse_pipe(token_lst, ast_node);
	while (token_lst && token_lst->type == TOKEN_AND)
	{
		token_lst = parse_pipe(token_lst->next, &right);
		left = *ast_node;
		*ast_node = create_binary_op_node(NODE_AND, left, right);
		right = NULL;
	}
	return (token_lst);
}

t_token_lst	*parse_or(t_token_lst *token_lst, t_ast_node **ast_node)
{
	t_ast_node	*left;
	t_ast_node	*right;

	right = NULL;
	token_lst= parse_and(token_lst, ast_node);
	while (token_lst && token_lst->type == TOKEN_OR)
	{
		token_lst = parse_and(token_lst->next, &right);
		left = *ast_node;
		*ast_node = create_binary_op_node(NODE_OR, left, right);
	}
	return (token_lst);
}
