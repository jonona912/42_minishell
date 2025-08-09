/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_utils_3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:24:19 by opopov            #+#    #+#             */
/*   Updated: 2025/05/08 11:04:55 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_tokenize_loop_part2_error_handler(int temp, t_tokenize_struct *vars,
		t_token_lst **token_lst, int *i)
{
	(void)i;
	if (temp < 0)
	{
		free(vars->current_token);
		token_free_list(*token_lst);
		return (0);
	}
	return (1);
}

int	handle_single_quote(char *current_token, char *line, char **final_word)
{
	int		temp;
	char	*temp_str;

	temp = copy_till_delimiter_2(current_token, line, '\'');
	if (temp == -1)
		return (-1);
	if (temp > 0)
	{
		temp_str = ft_strdup(current_token);
		append_to_final_word(final_word, temp_str, current_token);
	}
	return (temp);
}

int	handle_double_quote(char *current_token, char *line,
	char **final_word, t_shell *shell)
{
	int		temp;
	char	*temp_str;

	temp = copy_till_delimiter_2(current_token, line, '\"');
	if (temp == -1)
		return (-1);
	if (temp > 0)
	{
		temp_str = arg_word_return(current_token, *shell);
		append_to_final_word(final_word, temp_str, current_token);
	}
	return (temp);
}

void	process_and_append_to_final_word(char *current_token,
	char **final_word, t_shell *shell)
{
	char	*temp_str;

	temp_str = arg_word_return(current_token, *shell);
	if (*temp_str == '\0')
	{
		free(temp_str);
		*current_token = '\0';
		temp_str = NULL;
		return ;
	}
	append_to_final_word(final_word, temp_str, current_token);
}

int	copy_quotes_if(char *current_token, char *line,
	char **final_word, t_shell *shell)
{
	int	temp;
	int	i;

	i = 0;
	if (line[i] == '\"')
	{
		temp = handle_double_quote(current_token, line + i, final_word, shell);
		if (temp == -1)
			return (-1);
		i += temp;
	}
	if (line[i] == '\'')
	{
		temp = handle_single_quote(current_token, line + i, final_word);
		if (temp == -1)
			return (-1);
		i += temp;
	}
	return (i);
}
