#include "../../includes/minishell.h"

t_token_lst	*handle_parentheses(t_token_lst *token_lst,
		t_ast_node **ast_node, t_shell *shell)
{
	token_lst = token_lst->next;
	if (token_lst && token_lst->type == TOKEN_R_PAREN)
		return (ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO), NULL);
	token_lst = parse_or(token_lst, ast_node, shell);
	if (!token_lst)
	{
		return (NULL);
	}
	if (token_lst && token_lst->type != TOKEN_R_PAREN)
	{
		return (ft_putstr_fd("Error: no right parenthesis\n", STDERR_FILENO), NULL);
	}
	token_lst = token_lst->next;
	if (token_lst && (token_lst->type == TOKEN_AND || token_lst->type == TOKEN_OR || token_lst->type == TOKEN_PIPE || token_lst->type == TOKEN_END))
	{
		// return (ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO), NULL);
		return (token_lst);
	}
	// return (ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO), NULL);
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

t_token_lst	*populate_command_data(t_token_lst *token_lst,
		t_ast_node **ast_node, t_shell *shell)
{
	int			ctr;

	if (builtin_check(token_lst->value) || token_lst->type == TOKEN_ENV_VAR)
		(*ast_node)->data.cmd.executable = arg_return
			(token_lst->value, token_lst->type, shell);
	else
		(*ast_node)->data.cmd.executable = return_executable_path(token_lst->value, shell);
	// if (!(*ast_node)->data.cmd.executable)
	// {
	// 	ft_putstr_fd(token_lst->value, 2);
	// 	ft_putstr_fd(": command not found\n", 2);
	// 	shell->last_status = 127;
	// 	return (NULL);
	// }
	ctr = populate_command_data_loop1(token_lst);
	if (ctr > 0)// what about first argument (echo, cat ..)?
	{
		(*ast_node)->data.cmd.exec_argv = (char **)
			malloc((ctr + 1) * sizeof(char *));
		if (!(*ast_node)->data.cmd.exec_argv)
			return (NULL);
		token_lst = populate_command_data_loop2(token_lst, shell, *ast_node);
	}
	return (token_lst);
}

// int	is_cmd_valid(t_token_type type)
// {
// 	return (type == TOKEN_WORD || is_redirection(type) || type == TOKEN_ENV_VAR || type == TOKEN_D_QUOTE || type == TOKEN_S_QUOTE);
// }

// t_token_lst	*make_command_and_redirections_lists(t_token_lst *token_lst, t_token_lst **command_list, t_redir_lst **redirection_list)
// {
// 	t_token_lst	*temp;
// 	t_redir_lst	*redir_temp;
// 	t_token_type	type;

// 	while (is_cmd_valid(token_lst->type))
// 	{
// 		if (token_lst->type == TOKEN_WORD || token_lst->type == TOKEN_ENV_VAR)
// 		{
// 			temp = token_new_node(token_lst->type, ft_strdup(token_lst->value));
// 			if (!temp)
// 				return (NULL);
// 			token_add_node_back(command_list, temp);
// 		}
// 		else if (is_redirection(token_lst->type))
// 		{
// 			type = token_lst->type;
// 			token_lst = token_lst->next;
// 			if (token_lst && token_lst->type == TOKEN_WORD)
// 			{
// 				redir_temp = new_redir_node(type, ft_strdup(token_lst->value));
// 				// temp = token_new_node(type, ft_strdup(token_lst->value));
// 				if (!temp)
// 					return (NULL);
// 				add_redir_back(redirection_list, redir_temp);
// 				// token_add_node_back(redirection_list, temp);
// 			}
// 			else
// 			{
// 				return (ft_putstr_fd("minishell: syntax error near unexpected token\n", 2), NULL);
// 			}
// 		}
// 		else
// 		{
// 			return (ft_putstr_fd("minishell: syntax error near unexpected token\n", 2), NULL);
// 		}
// 		token_lst = token_lst->next;
// 	}
// 	return (token_lst);
// }

// int	set_executable_and_argv(t_ast_node **ast_node, t_token_lst *command_list, t_shell *shell)
// {
// 	int	argv_len;
// 	int	ctr;

// 	ctr = 0;
// 	argv_len = token_lst_size(command_list);
// 	(*ast_node)->data.cmd.exec_argv = (char **) malloc((argv_len + 1) * sizeof(char *));
// 	if (!(*ast_node)->data.cmd.exec_argv)
// 	{
// 		token_free_list(command_list);
// 		return (-1);
// 	}
// 	if (builtin_check(command_list->value))
// 		(*ast_node)->data.cmd.executable = arg_return(command_list->value, command_list->type, shell);
// 	else
// 		(*ast_node)->data.cmd.executable = return_executable_path(command_list->value);
// 	// (*ast_node)->data.cmd.executable = arg_return(command_list->value, command_list->type, shell);
// 	while (command_list)
// 	{
// 		(*ast_node)->data.cmd.exec_argv[ctr] = arg_return(command_list->value, command_list->type, shell);
// 		command_list = command_list->next;
// 		ctr++;
// 	}
// 	(*ast_node)->data.cmd.exec_argv[ctr] = NULL;
// 	return (0);
// }
	

// t_token_lst *parse_cmd(t_token_lst *token_lst, t_ast_node **ast_node, t_shell *shell)
// {
// 	int	is_cmd_created;
// 	t_token_lst	*command_list;

// 	command_list = NULL;
// 	is_cmd_created = 0;
// 	*ast_node = create_cmd_node(NODE_CMD, NULL, NULL, NULL);
// 	token_lst = make_command_and_redirections_lists(token_lst, &command_list, &(*ast_node)->data.cmd.redirs);
// 	if (!token_lst)
// 	{
// 		token_free_list(command_list);
// 		return (NULL);
// 	}
// 	if (set_executable_and_argv(ast_node, command_list, shell) == -1)
// 	{
// 		token_free_list(command_list);
// 		return (NULL);
// 	}
// 	return (token_lst);
// }

t_token_lst	*parse_word(t_token_lst *token_lst, t_ast_node **ast_node, t_shell *shell)
{
	if (token_lst && (token_lst->type == TOKEN_WORD || is_redirection(token_lst->type) || token_lst->type == TOKEN_ENV_VAR))
	{
		token_lst = parse_cmd(token_lst, ast_node, shell);
		// *ast_node = create_cmd_node(NODE_CMD, NULL, NULL, NULL);
		// token_lst = append_redirections_if_any
		// 	(token_lst, &(*ast_node)->data.cmd.redirs, shell);
		// if (!token_lst)
		// 	return (NULL);
		// token_lst = populate_command_data(token_lst, ast_node, shell); // problem
		// token_lst = append_redirections_if_any
		// 	(token_lst, &(*ast_node)->data.cmd.redirs, shell);
		return (token_lst);
	}
	if (token_lst && token_lst->type == TOKEN_L_PAREN)
	{
		token_lst = handle_parentheses(token_lst, ast_node, shell);
		if (!token_lst)
			return (NULL);
		*ast_node = create_subshell_node(NODE_SUBSHELL, *ast_node, NULL);
		token_lst = append_redirections_if_any
			(token_lst, &(*ast_node)->data.sub_shell.sub_shell_redir, shell);
		if (!token_lst)
			return (NULL);
	}
	return (token_lst);
}
