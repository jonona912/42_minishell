/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_utils_2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:20:22 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 18:24:37 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	send_paren_to_token_lst_paren(char *str, char *temp,
		t_tokenize_struct *vars)
{
	if (str[0] == '(')
		vars->paren_counter++;
	else if (str[0] == ')')
	{
		if (vars->paren_counter == 0)
		{
			free(temp);
			ft_putstr_fd("Error: unmatched parenthesis\n", 2);
			return (-1);
		}
		vars->paren_counter--;
	}
	return (0);
}

int	copy_till_delimiter_2(char *current_token, char *line, char delimiter)
{
	int	i;

	i = 0;
	if (line[i] == delimiter)
		i++;
	while (line[i] && line[i] != delimiter)
	{
		ft_append_char(current_token, line[i]);
		i++;
	}
	if (line[i] != delimiter)
	{
		ft_putstr_fd("Error: unmatched quotes\n", 2);
		return (-1);
	}
	i++;
	return (i);
}

void	wildards_to_token_lst(char *final_word, t_token_lst **token_lst)
{
	t_token_lst	*wildcard_lst;

	wildcard_lst = handle_wildcard_2(final_word);
	if (!wildcard_lst)
		append_to_token(TOKEN_WORD, final_word, token_lst);
	else
		token_add_node_back(token_lst, wildcard_lst);
}

void	append_to_final_word(char **final_word,
	char *temp_str, char *current_token)
{
	char	*tmp_2;

	if (*final_word == NULL)
	{
		*final_word = temp_str;
		temp_str = NULL;
	}
	else
	{
		tmp_2 = ft_strjoin(*final_word, temp_str);
		free(*final_word);
		*final_word = tmp_2;
		free(temp_str);
		temp_str = NULL;
	}
	*current_token = '\0';
}

int	append_final_word_to_token_lst(t_token_lst **token_lst,
		char *final_word, t_token_type type)
{
	if (final_word)
	{
		if (type == TOKEN_WILDCARD)
			wildards_to_token_lst(final_word, token_lst);
		else if (append_to_token(type, final_word, token_lst) == -1)
			return (-1);
		free(final_word);
	}
	return (0);
}
