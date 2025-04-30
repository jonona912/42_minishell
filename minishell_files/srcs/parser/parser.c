#include "../../includes/minishell.h"

int is_valid_after_poa(t_token_type type)
{
	return (type == TOKEN_WORD || type == TOKEN_L_PAREN);
}

int	is_valid_after_pipe(t_token_type type)
{
	return (type == TOKEN_WORD || type == TOKEN_L_PAREN
		|| type == TOKEN_S_QUOTE || type == TOKEN_D_QUOTE
		|| type == TOKEN_ENV_VAR || is_redirection(type));
}


t_token_lst	*parse_pipe(t_token_lst *token_lst,
		t_ast_node **ast_node, t_shell *shell)
{
	t_ast_node	*left;
	t_ast_node	*right;

	right = NULL;
	token_lst = parse_word(token_lst, ast_node, shell);
	while (token_lst && token_lst->type == TOKEN_PIPE)
	{
		token_lst = token_lst->next;
		if (token_lst && !is_valid_after_pipe(token_lst->type))
			return (ft_putstr_fd("minishell: incomplete command\n", STDERR_FILENO), NULL);
		token_lst = parse_word(token_lst, &right, shell);
		left = *ast_node;
		*ast_node = create_binary_op_node(NODE_PIPE, left, right);
		right = NULL;
	}
	return (token_lst);
}

t_token_lst	*parse_and(t_token_lst *token_lst,
		t_ast_node **ast_node, t_shell *shell)
{
	t_ast_node	*left;
	t_ast_node	*right;

	right = NULL;
	token_lst = parse_pipe(token_lst, ast_node, shell);
	while (token_lst && token_lst->type == TOKEN_AND)
	{
		token_lst = token_lst->next;
		if (token_lst && !is_valid_after_poa(token_lst->type))
			return (ft_putstr_fd("minishell: incomplete command\n", STDERR_FILENO), NULL);
		token_lst = parse_pipe(token_lst, &right, shell);
		left = *ast_node;
		*ast_node = create_binary_op_node(NODE_AND, left, right);
		right = NULL;
	}
	return (token_lst);
}

t_token_lst	*parse_or(t_token_lst *token_lst,
		t_ast_node **ast_node, t_shell *shell)
{
	t_ast_node	*left;
	t_ast_node	*right;

	right = NULL;
	token_lst = parse_and(token_lst, ast_node, shell);
	while (token_lst && token_lst->type == TOKEN_OR)
	{
		token_lst = token_lst->next;
		if (token_lst && !is_valid_after_poa(token_lst->type))
			return (ft_putstr_fd("minishell: incomplete command\n", STDERR_FILENO), NULL);
		token_lst = parse_and(token_lst, &right, shell);
		left = *ast_node;
		*ast_node = create_binary_op_node(NODE_OR, left, right);
	}
	return (token_lst);
}
