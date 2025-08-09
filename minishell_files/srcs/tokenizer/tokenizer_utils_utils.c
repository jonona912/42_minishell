/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 11:00:01 by opopov            #+#    #+#             */
/*   Updated: 2025/05/08 11:03:24 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_ampersand_error(char *line, char **final_word)
{
	if (line == NULL)
		return (0);
	if (line[0] && line[0] == '&' && line[1] != '&')
	{
		ft_putstr_fd("Error: single '&' is not allowed\n", 2);
		free(*final_word);
		return (-1);
	}
	return (0);
}

int	is_char_valid_for_word(char c)
{
	return (c && !ft_isblank(c) && ft_strchr("<>|&()", c) == NULL);
}

int	create_word_token(char *current_token, char *line,
	t_token_lst **token_lst, t_shell *shell)
{
	int				i;
	int				temp;
	t_token_type	type;
	char			*final_word;

	final_word = NULL;
	type = TOKEN_WORD;
	i = 0;
	while (is_char_valid_for_word(line[i]))
	{
		temp = copy_until_special_char(current_token, line + i, "<>|&()\"\'");
		i += temp;
		if (is_wildcard_present(line))
			type = TOKEN_WILDCARD;
		if (temp > 0)
			process_and_append_to_final_word(current_token, &final_word, shell);
		temp = copy_quotes_if(current_token, line + i, &final_word, shell);
		if (temp == -1)
			return (free(final_word), -1);
		i += temp;
	}
	if (handle_ampersand_error(line + i, &final_word) == -1
		|| append_final_word_to_token_lst(token_lst, final_word, type) == -1)
		return (-1);
	return (i);
}

int	is_wildcard_present(char *line)
{
	int	i;

	if (!line || line[0] == '\0')
		return (0);
	i = 0;
	while (is_not_special_char(line[i]))
	{
		if (line[i] == '\0')
			return (0);
		if (line[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

int	copy_until_special_char(char *current_token, char *line, char *char_set)
{
	int	i;

	i = 0;
	while (line[i] && !ft_isblank(line[i])
		&& ft_strchr(char_set, line[i]) == NULL)
	{
		ft_append_char(current_token, line[i]);
		i++;
	}
	return (i);
}
