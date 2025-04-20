#include "../../includes/minishell.h"

t_ast_node	*return_rightest_node(t_ast_node *ast_node)
{
	t_ast_node	*temp;

	if (!ast_node)
		return (NULL);
	temp = ast_node;
	while (temp && temp->type != NODE_CMD)
	{
		temp = temp->data.binary_op.right;
	}
	return (temp);
}

t_token_lst	*handle_parentheses(t_token_lst *token_lst, t_ast_node **ast_node)
{
	token_lst = token_lst->next;
	token_lst = parse_or(token_lst, ast_node);
	if (token_lst->type != TOKEN_R_PAREN)
	{
		return (ft_putstr_fd("Error: no left parent\n", STDERR_FILENO), NULL); // handle error
	}
	token_lst = token_lst->next;
	if (token_lst && is_redirection(token_lst->type))
	{
		t_ast_node	*temp_node = return_rightest_node(*ast_node);
		if (!temp_node)
			return (NULL); // handle error
		token_lst = append_redirections(&temp_node, token_lst);
		if (!token_lst)
			return (NULL); // handle error
	}
	return (token_lst);
}

t_token_lst	*append_redirections_if_any(t_token_lst *token_lst, t_ast_node **ast_node)
{
	if (token_lst && is_redirection(token_lst->type))
	{
		token_lst = append_redirections(ast_node, token_lst);
		if (!token_lst)// || token_lst->type != TOKEN_WORD)
		{
			return (ft_putstr_fd("minishell: syntax error near unexpected token\n", 2), NULL); // handle error
		}
	}
	return (token_lst);
}

int	is_word_or_quote(t_token_type type)
{
	return (type == TOKEN_WORD || type == TOKEN_S_QUOTE || type == TOKEN_D_QUOTE);
}

int count_token_words(t_token_lst *token_lst)
{
	int count = 0;

	while (token_lst && token_lst->type == TOKEN_WORD) // you can copy, double quote, single quote
	{
		count++;
		// if wildcard is met
		token_lst = token_lst->next;
	}
	return count;
}

t_token_lst	*populate_command_data(t_token_lst *token_lst, t_ast_node **ast_node)
{
	t_token_lst	*current_token;
	int			ctr;

	(*ast_node)->data.cmd.executable = return_executable_path(token_lst->value);
	if (!(*ast_node)->data.cmd.executable)
		return (perror("minishell:"), NULL);
	current_token = token_lst;
	ctr = 0;
	while (current_token && current_token->type == TOKEN_WORD) // you can copy, double quote, single quote
	{
		ctr++;
		// if wildcard is met
		current_token = current_token->next;
	}
	(*ast_node)->data.cmd.exec_argv = (char **)malloc((ctr + 1) * sizeof(char *)); // if fails?
	if (!(*ast_node)->data.cmd.exec_argv)
		return (NULL); // handle malloc failure
	ctr = 0;
	while (token_lst && token_lst->type == TOKEN_WORD)
	{
		// if (ft_strchr(token_lst->value, '*'))
		
		(*ast_node)->data.cmd.exec_argv[ctr] = ft_strdup(token_lst->value);
		if (!(*ast_node)->data.cmd.exec_argv[ctr])
			return (NULL); // handle strdup failure
		token_lst = token_lst->next;
		ctr++;
	}
	(*ast_node)->data.cmd.exec_argv[ctr] = NULL;
	return (token_lst);
}


t_token_lst	*parse_word(t_token_lst *token_lst, t_ast_node **ast_node)
{
	// t_token_lst	*current_token;
	// int			ctr;
	// t_token_lst	*wildcard_token;

	// ctr = 0;
	if (token_lst && (token_lst->type == TOKEN_WORD || is_redirection(token_lst->type)))
	{
		*ast_node = create_cmd_node(NODE_CMD, NULL, NULL, NULL); // handle if failure happens
		token_lst = append_redirections_if_any(token_lst, ast_node);
		if (!token_lst)
			return (NULL);
		token_lst = populate_command_data(token_lst, ast_node);
		// (*ast_node)->data.cmd.executable = return_executable_path(token_lst->value);
		// current_token = token_lst;
		// while (current_token && current_token->type == TOKEN_WORD) // you can copy, double quote, single quote
		// {
		// 	ctr++;
		// 	// if wildcard is met
		// 	current_token = current_token->next;
		// }
		// (*ast_node)->data.cmd.exec_argv = (char **)malloc((ctr + 1) * sizeof(char *)); // if fails?
		// ctr = 0;
		// while (token_lst && token_lst->type == TOKEN_WORD)
		// {
		// 	// if (ft_strchr(token_lst->value, '*'))
			
		// 	(*ast_node)->data.cmd.exec_argv[ctr] = ft_strdup(token_lst->value);
		// 	token_lst = token_lst->next;
		// 	ctr++;
		// }
		// (*ast_node)->data.cmd.exec_argv[ctr] = NULL;
		token_lst = append_redirections_if_any(token_lst, ast_node);
		return (token_lst);
	}
	if (token_lst && token_lst->type == TOKEN_L_PAREN)
	{
		token_lst = handle_parentheses(token_lst, ast_node);
	}
	return (token_lst);
}
