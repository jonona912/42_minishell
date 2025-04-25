/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:12:05 by opopov            #+#    #+#             */
/*   Updated: 2025/04/25 11:24:43 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*copy_token_till_delimeter_step1(char **dest, char delimiter, int *j)
{
	char	*temp;

	(*dest)[*j] = delimiter;
	(*dest)[++(*j)] = '\0';
	temp = ft_strdup(*dest);
	if (!temp)
	{
		ft_putstr_fd("Error: failed allocation memory", 2);
		return (NULL);
	}
	**dest = '\0';
	return (temp);
}

int	copy_token_till_delimeter_step2(char *src, int i,
	char *temp, t_token_lst **token_lst)
{
	t_token_lst	*temp_lst;

	temp_lst = token_new_node(return_token_type(src + i), temp);
	if (!temp_lst)
	{
		free(temp);
		ft_putstr_fd("Error: failed allocation memory", 2);
		return (-1);
	}
	token_add_node_back(token_lst, temp_lst);
	return (0);
}

char	**copy_token_till_delimeter_dest(char **dest, int *i, int *j, char *src)
{
	*i = 0;
	*j = 0;
	while ((*dest)[*j])
		(*j)++;
	(*dest)[(*j)++] = src[(*i)++];
	return (dest);
}

int	send_paren_to_token_lst_paren(char *str, char *temp, t_tokenize_struct *vars)
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

int	ft_tokenize_loop_part1(char *line, int *i,
		t_tokenize_struct *vars, t_token_lst **token_lst)
{
	int	temp;

	while (ft_isblank(line[*i]))
		(*i)++;
	temp = handle_quotes(vars, line + *i, token_lst);
	if (temp < 0)
	{
		free(vars->current_token);
		token_free_list(*token_lst);
		return (0);
	}
	*i += temp;
	return (1);
}

int	ft_tokenize_loop_part2(char *line, int *i,
		t_tokenize_struct *vars, t_token_lst **token_lst)
{
	int	temp;

	temp = handle_other_tokens(line + *i, token_lst, vars);
	if (temp < 0)
	{
		free(vars->current_token);
		token_free_list(*token_lst);
		return (0);
	}
	*i += temp;
	while (ft_isblank(line[*i]))
		(*i)++;
	temp = handle_env_var(vars->current_token, line + *i, token_lst);
	if (temp < 0)
	{
		token_free_list(*token_lst);
		free(vars->current_token);
		return (0);
	}
	*i += temp;
	temp = handle_wildcard(vars->current_token, line + *i, token_lst);
	if (temp < 0)
	{
		free(vars->current_token);
		token_free_list(*token_lst);
		return (0);
	}
	*i += temp;
	temp = create_word_token(vars->current_token, line + *i, token_lst);
	if (temp < 0)
	{
		token_free_list(*token_lst);
		free(vars->current_token);
		return (0);
	}
	*i += temp;
	return (1);
}
