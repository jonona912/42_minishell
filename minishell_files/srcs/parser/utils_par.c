/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_par.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:48:41 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 14:48:42 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
