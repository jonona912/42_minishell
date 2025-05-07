/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 11:00:01 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 12:07:56 by zkhojazo         ###   ########.fr       */
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
	t_token_lst *wildcard_lst;

	wildcard_lst = handle_wildcard_2(final_word);
	if (!wildcard_lst)
		append_to_token(TOKEN_WORD, final_word, token_lst);
	else
		token_add_node_back(token_lst, wildcard_lst);
}

void	append_to_final_word(char **final_word, char *temp_str, char *current_token)
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
// temp = copy_till_delimiter_2(current_token, line + i, '\"');
// if (temp == -1)
// 	return (-1);
// i += temp;
// if (temp > 0)
// {
// 	temp_str = arg_word_return(current_token, *shell);
// 	append_to_final_word(&final_word, temp_str, current_token);
// }
int	handle_double_quote(char *current_token, char *line, char **final_word, t_shell *shell)
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

void	process_and_append_to_final_word(char *current_token, char **final_word, t_shell *shell)
{
	char	*temp_str;

	temp_str = arg_word_return(current_token, *shell);
	if (*temp_str == '\0')
	{
		free(temp_str);
		*current_token = '\0';
		temp_str = NULL;
		return;
	}
	append_to_final_word(final_word, temp_str, current_token);
}

int	handle_quotes_if_exists(char *current_token, char *line, char **final_word, t_shell *shell)
{
	int	temp;
	int	i;

	i = 0;
	if (line[i] == '\"')
	{
		temp = handle_double_quote(current_token, line + i, final_word, shell);
		if (temp == -1)
			return (free(final_word), -1);
		i += temp;
	}
	if (line[i] == '\'')
	{
		temp = handle_single_quote(current_token, line + i, final_word);
		if (temp == -1)
			return (free(final_word), -1);
		i += temp;
	}
	return (i);
}

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

int	create_word_token(char *current_token, char *line, t_token_lst **token_lst, t_shell *shell)
{
	int				i;
	int				temp;
	t_token_type	type;
	char			*final_word;

	final_word = NULL;
	type = TOKEN_WORD;
	i = 0;
	while (line[i] && !ft_isblank(line[i]) && ft_strchr("<>|&()", line[i]) == NULL)
	{
		temp = copy_until_special_char(current_token, line + i, "<>|&()\"\'");
		i += temp;
		if (is_wildcard_present(line))
			type = TOKEN_WILDCARD;
		if (temp > 0)
			process_and_append_to_final_word(current_token, &final_word, shell);
		temp = handle_quotes_if_exists(current_token, line + i, &final_word, shell);
		if (temp == -1)
			return (free(final_word), -1);
		i += temp;
	}
	if (handle_ampersand_error(line + i, &final_word) == -1 ||
		append_final_word_to_token_lst(token_lst, final_word, type) == -1)
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
