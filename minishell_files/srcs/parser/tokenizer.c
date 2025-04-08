/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:43:18 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/08 12:17:00 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_quotes(t_tokenize_struct *vars, char *line, int *i)
{
	if (line[*i] == '\"' && !vars->is_s_quote)
	{
		vars->is_d_quote = !vars->is_d_quote;
		ft_append_char(vars->current_token, line[*i]);
		(*i)++;
		return (1);
	}
	if (line[*i] == '\'' && !vars->is_d_quote)
	{
		vars->is_s_quote = !vars->is_s_quote;
		ft_append_char(vars->current_token, line[*i]);
		(*i)++;
		return (1);
	}
	if (vars->is_s_quote || vars->is_d_quote)
	{
		ft_append_char(vars->current_token, line[*i]);
		(*i)++;
		return (1);
	}
	return (0);
}

int	process_redirection(t_tokenize_struct *vars, t_token_lst **token_lst, char *line, int *i, t_token_type token_type, int step)
{
	char	*temp;

	if (vars->current_token[0] != '\0')
	{
		temp = ft_strdup(vars->current_token);
		token_add_node_back(token_lst, token_new_node(0, temp));
		vars->current_token[0] = '\0';
	}
	ft_append_char(vars->current_token, line[*i]);
	if (step == 2)
		ft_append_char(vars->current_token, line[*i + 1]);
	temp = ft_strdup(vars->current_token);
	token_add_node_back(token_lst, token_new_node(token_type, temp));
	vars->current_token[0] = '\0';
	*i += step;
	return (1);
}

int	handle_redirection(t_tokenize_struct *vars, t_token_lst **token_lst, char *line, int *i)
{
	if (line[*i] == '<')
		return (process_redirection(vars, token_lst, line, i, TOKEN_REDIRECTION_IN , 1));
	if (line[*i] == '>')
		return (process_redirection(vars, token_lst, line, i, TOKEN_REDIRECTION_OUT , 1));
	if (line[*i] == '>' && line[*i + 1] == '>')
		return (process_redirection(vars, token_lst, line, i, TOKEN_APPEND, 2));
	if (line[*i] == '|')
		return (process_redirection(vars, token_lst, line, i, TOKEN_PIPE, 1));
	return (0);
}

int	handle_whitespace(t_tokenize_struct *vars, t_token_lst **token_lst, char *line, int *i)
{
	char	*temp;

	if (ft_isblank(line[*i]))
	{
		while (ft_isblank(line[*i]))
			(*i)++;
		if (vars->current_token[0] == '\0')
			return (1);
		temp = ft_strdup(vars->current_token); // check if not NULL?
		token_add_node_back(token_lst, token_new_node(0, temp));
		vars->current_token[0] = '\0';
		return (1);
	}
	return (0);
}

int	handle_unmatched_quotes(t_tokenize_struct *vars, t_token_lst **token_lst)
{
	if (vars->current_token[0] != '\0')
	{
		char *temp = ft_strdup(vars->current_token);
		token_add_node_back(token_lst, token_new_node(0, temp));
	}
	free(vars->current_token);
	if (vars->is_s_quote || vars->is_d_quote)
	{
		printf("Error: unmatched quotes\n");
		token_free_list(*token_lst);
		return (1);
	}
	return (0);
}

t_token_lst	*ft_tokenize(char *line)
{
	t_token_lst			*token_lst;
	t_tokenize_struct	vars;
	// char				*temp;
	int					i;

	initialize_tokenize_struct(&vars, line);
	token_lst = NULL;
	i = 0;
	while (ft_isspace(line[i]))
		i++;
	while (line[i])
	{
		if (handle_quotes(&vars, line, &i))
			continue ;
		if (handle_whitespace(&vars, &token_lst, line, &i))
			continue ;
		if (handle_redirection(&vars, &token_lst, line, &i)) // pipe also handled
			continue ;
		ft_append_char(vars.current_token, line[i]);
		i++;
	}
	if (handle_unmatched_quotes(&vars, &token_lst))
		return (NULL);
	return (token_lst);
}
