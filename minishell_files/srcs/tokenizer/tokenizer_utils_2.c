/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:19:56 by opopov            #+#    #+#             */
/*   Updated: 2025/04/26 11:03:35 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token_type	return_token_type(char *str)
{
	if (*str == '<')
		return (TOKEN_REDIRECTION_IN);
	if (*str == '>')
		return (TOKEN_REDIRECTION_OUT);
	if (*str == '|')
		return (TOKEN_PIPE);
	if (*str == '\"')
		return (TOKEN_D_QUOTE);
	if (*str == '\'')
		return (TOKEN_S_QUOTE);
	if (*str == '(')
		return (TOKEN_L_PAREN);
	if (*str == ')')
		return (TOKEN_R_PAREN);
	if (*str == '$')
		return (TOKEN_ENV_VAR);
	if (ft_strncmp(str, ">>", 2) == 0)
		return (TOKEN_APPEND);
	if (ft_strncmp(str, "<<", 2) == 0)
		return (TOKEN_HEREDOC);
	if (ft_strncmp(str, "&&", 2) == 0)
		return (TOKEN_AND);
	if (ft_strncmp(str, "||", 2) == 0)
		return (TOKEN_OR);
	return (0);
}

int	send_str_to_token_lst(char *str, t_token_lst **token_lst,
		t_token_type token_type)
{
	char	*temp;

	if (str[0] != '\0')
	{
		temp = ft_strdup(str);
		if (!temp)
		{
			ft_putstr_fd("Error: failed allocation memory", 2);
			return (-1);
		}
		token_add_node_back(token_lst, token_new_node(token_type, temp));
	}
	return (ft_strlen(str));
}

int	handle_env_var(char *current_token, char *line, t_token_lst **token_lst)
{
	int	i;

	if (!line)
		return (-1);
	if (line[0] == '\0')
		return (0);
	i = 0;
	if (*line == '$')
	{
		i = copy_until_special_char(current_token, line, "<>|&()\"\'");
	}
	if (current_token[0] != '\0')
	{
		if (append_to_token(TOKEN_ENV_VAR, current_token, token_lst) == -1)
			return (-1);
	}
	return (i);
}

int	send_paren_to_token_lst(char *str, t_token_lst **token_lst,
	t_token_type token_type, t_tokenize_struct *vars)
{
	char		*temp;
	t_token_lst	*temp_lst;

	temp_lst = NULL;
	temp = ft_strdup(str);
	if (!temp)
	{
		ft_putstr_fd("Error: failed allocation memory", 2);
		return (-1);
	}
	if (send_paren_to_token_lst_paren(str, temp, vars) == -1)
		return (-1);
	temp_lst = token_new_node(token_type, temp);
	if (!temp_lst)
	{
		free(temp);
		ft_putstr_fd("Error: failed allocation memory", 2);
		return (-1);
	}
	token_add_node_back(token_lst, temp_lst);
	return (1);
}

int	copy_token_till_delimiter(char **dest, char *src,
	char delimiter, t_token_lst **token_lst)
{
	int			i;
	int			j;
	char		*temp;

	dest = copy_token_till_delimeter_dest(dest, &i, &j, src);
	while (src[i] && (src[i] != delimiter))
	{
		(*dest)[j++] = src[i++];
		if (src[i] && (src[i] == delimiter && src[i + 1] == delimiter))
			i += 2;
	}
	(*dest)[j] = '\0';
	if (src[i] == delimiter)
	{
		temp = copy_token_till_delimeter_step1(dest, delimiter, &j);
		if (!temp)
			return (-1);
		if (copy_token_till_delimeter_step2(src, i, temp, token_lst) < 0)
			return (-1);
		return (++i);
	}
	ft_putstr_fd("Error: unmatched quotes\n", 2);
	return (-1);
}
