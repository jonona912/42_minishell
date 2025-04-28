/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 11:07:58 by opopov            #+#    #+#             */
/*   Updated: 2025/04/28 19:07:35 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_wildcard_type	find_wildcard_type(char *line)
{
	t_wildcard_type	wildcard_type;
	int				i;

	i = 0;
	wildcard_type = WILDCARD_NONE;
	while (is_not_special_char(line[i]) && line[i] == '*')
	{
		wildcard_type = WILDCARD_ONLY;
		i++;
	}
	while (is_not_special_char(line[i]) && line[i] != '*')
	{
		if (wildcard_type == WILDCARD_ONLY)
			wildcard_type = WILDCARD_START;
		i++;
	}
	while (is_not_special_char(line[i]) && line[i] == '*')
	{
		if (wildcard_type == WILDCARD_NONE)
			wildcard_type = WILDCARD_END;
		i++;
	}
	return (wildcard_type);
}

int	copy_wildcard_string(char **dest, char *line)
{
	int		i;
	char	*traverse;
	int		len;

	len = 0;
	i = 0;
	while (is_not_special_char(line[i]) && line[i] == '*')
		i++;
	traverse = line + i;
	while (is_not_special_char(*traverse) && *traverse != '*')
	{
		len++;
		traverse++;
	}
	*dest = (char *)malloc(len + 1);
	if (!*dest)
	{
		perror("malloc");
		return (-1);
	}
	i = copy_wildcard_string_loop(line, i, dest);
	return (i);
}

t_wildcard_type_string	*identify_wildcard_type(char *line, int *char_ctr)
{
	t_wildcard_type_string	*wildcard_type_string;

	wildcard_type_string = (t_wildcard_type_string *)
		malloc(sizeof(t_wildcard_type_string));
	if (!wildcard_type_string)
	{
		perror("malloc");
		return (NULL);
	}
	if (!line || line[0] == '\0')
	{
		wildcard_type_string->type = WILDCARD_NONE;
		return (wildcard_type_string);
	}
	wildcard_type_string->type = find_wildcard_type(line);
	identify_wildcard_type_if(wildcard_type_string, char_ctr, line);
	return (wildcard_type_string);
}

t_token_lst	*join_wildcar_token(t_read_dir *read_dir,
	t_wildcard_type_string *wildcard_string,
	char *(ft_strstr_func)(const char *str, const char *wildcard))
{
	t_token_lst	*wildcard_list;

	wildcard_list = NULL;
	while (1)
	{
		read_dir->entry = readdir(read_dir->dir);
		if (read_dir->entry == NULL)
			break ;
		if (stat(read_dir->entry->d_name, &read_dir->file_stat))
		{
			perror("stat");
			continue ;
		}
		if (read_dir->entry->d_name[0] == '.')
			continue ;
		join_wildcar_token_if(ft_strstr_func, read_dir,
			wildcard_string, &wildcard_list);
	}
	return (wildcard_list);
}

t_token_lst	*wildcard_function(char *line, int *char_ctr)
{
	t_read_dir				read_dir;
	t_token_lst				*wildcard_list;
	t_wildcard_type_string	*wildcard_string;

	wildcard_string = identify_wildcard_type(line, char_ctr);
	wildcard_list = NULL;
	read_dir.dir = opendir(".");
	if (read_dir.dir == NULL)
	{
		perror("opendir");
		return (NULL);
	}
	wildcard_list = wildcard_function_if(wildcard_string,
			wildcard_list, &read_dir);
	if (closedir(read_dir.dir))
	{
		perror("closedir");
		return (NULL);
	}
	if (wildcard_string->data)
		free(wildcard_string->data);
	free(wildcard_string);
	return (wildcard_list);
}
