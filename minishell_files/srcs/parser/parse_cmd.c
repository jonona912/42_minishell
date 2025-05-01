#include "../../includes/minishell.h"

int	is_cmd_valid(t_token_type type)
{
	return (type == TOKEN_WORD
		|| is_redirection(type)
		|| type == TOKEN_ENV_VAR
		|| type == TOKEN_D_QUOTE
		|| type == TOKEN_S_QUOTE);
}

t_token_lst	*handle_cmd_redir(t_token_lst *token_lst,
								t_redir_lst **redir_lst, t_shell *shell)
{
	t_redir_lst		*redir_temp;
	t_token_type	type;
	char			*temp_str;

	type = token_lst->type;
	token_lst = token_lst->next;
	if (token_lst && (token_lst->type == TOKEN_WORD || token_lst->type == TOKEN_S_QUOTE ||
		token_lst->type == TOKEN_D_QUOTE))
	{
		temp_str = arg_return(token_lst->value, token_lst->type, shell);
		if (!temp_str)
			return (NULL);
		redir_temp = new_redir_node(type, temp_str);
		if (!redir_temp)
		{
			free(temp_str);
			return (NULL);
		}
		add_redir_back(redir_lst, redir_temp);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error2\n", 2);
		return (NULL);
	}
	return (token_lst);
}

t_token_lst	*make_cmd_and_redir_lst(t_token_lst *token_lst,
									t_token_lst **cmd_lst,
									t_redir_lst **redir_lst, t_shell *shell)
{
	t_token_lst		*temp;
	char			*temp_str;
	while (is_cmd_valid(token_lst->type))
	{
		if (token_lst->type == TOKEN_WORD || token_lst->type == TOKEN_ENV_VAR
			|| token_lst->type == TOKEN_D_QUOTE || token_lst->type == TOKEN_S_QUOTE)
		{
			temp_str = arg_return(token_lst->value, token_lst->type, shell);
			temp = token_new_node(token_lst->type, temp_str); // potential fix 
			if (!temp)
				return (NULL);
			token_add_node_back(cmd_lst, temp);
		}
		else if (is_redirection(token_lst->type))
		{
			token_lst = handle_cmd_redir(token_lst, redir_lst, shell);
			if (!token_lst)
				return (NULL);
		}
		else
			return (ft_putstr_fd("minishell: syntax error\n", 2), NULL);
		token_lst = token_lst->next;
	}
	return (token_lst);
}

int	set_executable_and_argv(t_ast_node **ast_node,
							t_token_lst *cmd_lst,
							t_shell *shell)
{
	int	ctr;

	if (!cmd_lst)
		return (0);
	ctr = token_lst_size(cmd_lst);
	if (ctr == 0)
		return (0);
	(*ast_node)->data.cmd.exec_argv
		= (char **) malloc((ctr + 1) * sizeof(char *));
	if (!(*ast_node)->data.cmd.exec_argv)
		return (-1);
	if (builtin_check(cmd_lst->value))
		(*ast_node)->data.cmd.executable
			= arg_return(cmd_lst->value, cmd_lst->type, shell);
	else
		(*ast_node)->data.cmd.executable
			= return_executable_path(cmd_lst->value, shell); // fix path here
	ctr = 0;
	while (cmd_lst)
	{
		(*ast_node)->data.cmd.exec_argv[ctr]
			= arg_return(cmd_lst->value, cmd_lst->type, shell);
		cmd_lst = cmd_lst->next;
		ctr++;
	}
	return ((*ast_node)->data.cmd.exec_argv[ctr] = NULL, 0);
}

t_token_lst	*parse_cmd(t_token_lst *token_lst,
						t_ast_node **ast_node,
						t_shell *shell)
{
	// int			is_cmd_created;
	t_token_lst	*cmd_lst;

	cmd_lst = NULL;
	// is_cmd_created = 0;
	*ast_node = create_cmd_node(NODE_CMD, NULL, NULL, NULL);
	token_lst = make_cmd_and_redir_lst(token_lst,
			&cmd_lst, &(*ast_node)->data.cmd.redirs, shell);
	if (!token_lst)
	{
		token_free_list(cmd_lst);
		return (NULL);
	}
	if (set_executable_and_argv(ast_node, cmd_lst, shell) == -1)
	{
		token_free_list(cmd_lst);
		return (NULL);
	}
	token_free_list(cmd_lst);
	return (token_lst);
}
