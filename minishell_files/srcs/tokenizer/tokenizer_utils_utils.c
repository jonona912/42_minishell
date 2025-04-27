/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 11:00:01 by opopov            #+#    #+#             */
/*   Updated: 2025/04/26 11:04:49 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_tokenize_loop_part2_error_handler(int temp, t_tokenize_struct *vars,
		t_token_lst **token_lst, int *i)
{
	if (temp < 0)
	{
		free(vars->current_token);
		token_free_list(*token_lst);
		return (0);
	}
	*i += temp;
	return (1);
}

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
			ft_putstr_fd("Error: unmatched parenthesis", 2);
			return (-1);
		}
		vars->paren_counter--;
	}
	return (0);
}

int	create_word_token(char *current_token, char *line, t_token_lst **token_lst)
{
	int		i;

	i = copy_until_special_char(current_token, line, "<>|&()\"\'");
	if (current_token[0] != '\0')
	{
		if (append_to_token(TOKEN_WORD, current_token, token_lst) == -1)
			return (-1);
	}
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
