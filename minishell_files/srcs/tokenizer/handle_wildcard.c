/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:45:00 by opopov            #+#    #+#             */
/*   Updated: 2025/05/08 12:25:03 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	match_pattern(const char *str, const char *pattern)
{
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == '\0')
		return (0);
	if (*str == '\0' && *pattern != '*')
		return (0);
	if (*pattern != '*')
	{
		if (*str == *pattern || (*pattern == '.' && *str == '.'))
			return (match_pattern(str + 1, pattern + 1));
		return (0);
	}
	while (1)
	{
		while (*pattern == '*')
			pattern++;
		if (match_pattern(str, pattern))
			return (1);
		if (*str == '\0')
			break ;
		str++;
	}
	return (0);
}

int	is_match(const char *filename, const char *pattern)
{
	return (match_pattern(filename, pattern));
}

t_token_lst	*handle_wildcard_2(char *line)
{
	t_read_dir				read_dir;
	t_token_lst				*wildcard_list;

	wildcard_list = NULL;
	read_dir.dir = opendir(".");
	if (read_dir.dir == NULL)
	{
		perror("opendir");
		return (NULL);
	}
	read_dir.entry = readdir(read_dir.dir);
	while (read_dir.entry != NULL)
	{
		if (is_match(read_dir.entry->d_name, line))
			token_add_node_back(&wildcard_list, token_new_node
				(TOKEN_WORD, ft_strdup(read_dir.entry->d_name)));
		read_dir.entry = readdir(read_dir.dir);
	}
	if (closedir(read_dir.dir))
	{
		perror("closedir");
		return (NULL);
	}
	return (wildcard_list);
}

int	check_for_space(char *line)
{
	int	c;
	int	is_space;
	int	line_len;

	line_len = ft_strlen(line);
	if (line_len == 0)
		return (1);
	c = 0;
	is_space = 0;
	while (c < line_len)
	{
		if (ft_isspace(line[c]))
			is_space++;
		c++;
	}
	if (is_space == c)
		return (1);
	return (0);
}
