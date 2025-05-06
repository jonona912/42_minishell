#include "includes/parser.h"

void	extract_wildcard_content(t_wildcard_type_string *wildcard_type_string,
	int *char_ctr, char *line)
{
	if (wildcard_type_string->type == WILDCARD_START
		|| wildcard_type_string->type == WILDCARD_END)
		*char_ctr = copy_wildcard_string(&wildcard_type_string->data, line);
	else if (wildcard_type_string->type == WILDCARD_ONLY)
	{
		while (is_not_special_char(line[*char_ctr]) && line[*char_ctr] == '*')
			(*char_ctr)++;
		wildcard_type_string->data = NULL;
	}
	else
		wildcard_type_string->data = NULL;
}

t_token_lst	*handle_wildcard_variants(t_wildcard_type_string *wildcard_string,
		t_token_lst *wildcard_list, t_read_dir *read_dir)
{
	if (wildcard_string->type == WILDCARD_START)
		wildcard_list = join_wildcar_token(read_dir,
				wildcard_string, ft_strstr_last);
	else if (wildcard_string->type == WILDCARD_END)
		wildcard_list = join_wildcar_token(read_dir,
				wildcard_string, ft_strstr_first);
	else if (wildcard_string->type == WILDCARD_ONLY)
		wildcard_list = join_wildcar_token(read_dir,
				wildcard_string, NULL);
	return (wildcard_list);
}

void	add_matching_wildcard_token(char *(ft_strstr_func)
		(const char *str, const char *wildcard),
		t_read_dir *read_dir, t_wildcard_type_string *wildcard_string,
		t_token_lst **wildcard_list)
{
	char	*str;

	if (ft_strstr_func)
	{
		str = ft_strstr_func(read_dir->entry->d_name,
				wildcard_string->data);
		if (str)
		{
			str = ft_strdup(read_dir->entry->d_name);
			token_add_node_back(wildcard_list,
				token_new_node(TOKEN_WORD, str));
		}
	}
	else
	{
		str = ft_strdup(read_dir->entry->d_name);
		token_add_node_back(wildcard_list,
			token_new_node(TOKEN_WORD, str));
	}
}

// t_token_lst	*populate_command_data_loop2(t_token_lst *token_lst, t_shell *shell,
// 		t_ast_node *ast_node)
// {
// 	int			ctr;

// 	ctr = 0;
// 	while (token_lst && (token_lst->type == TOKEN_ENV_VAR
// 			|| token_lst->type == TOKEN_WORD || token_lst->type == TOKEN_D_QUOTE
// 			|| token_lst->type == TOKEN_S_QUOTE))
// 	{
// 		ast_node->data.cmd.exec_argv[ctr] = arg_return
// 			(token_lst->value, token_lst->type, shell);
// 		if (!ast_node->data.cmd.exec_argv[ctr])
// 			return (NULL);
// 		token_lst = token_lst->next;
// 		ctr++;
// 	}
// 	ast_node->data.cmd.exec_argv[ctr] = NULL;
// 	return (token_lst);
// }

// int	populate_command_data_loop1(t_token_lst *token_lst)
// {
// 	t_token_lst	*current_token;
// 	int			ctr;

// 	current_token = token_lst;
// 	ctr = 0;
// 	while (current_token && (token_lst->type == TOKEN_ENV_VAR
// 			|| token_lst->type == TOKEN_WORD || token_lst->type == TOKEN_D_QUOTE
// 			|| token_lst->type == TOKEN_S_QUOTE))
// 	{
// 		ctr++;
// 		current_token = current_token->next;
// 	}
// 	return (ctr);
// }
