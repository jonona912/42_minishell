/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:43:18 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/05 15:50:03 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_isblank(int c)
{
	return (c == ' ' || c == '\t');
}

int	ft_append_char(char *str, char c)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	str[i] = c;
	str[i + 1] = '\0';
	return (1);
}

void	initialize_tokenize_struct(t_tokenize_struct *vars, char *line)
{
	int	line_len;

	line_len = strlen(line);
	vars->current_token = (char *)malloc((line_len + 1) * sizeof(char));
	if (!vars->current_token)
	{
		printf("Error: memory allocation failed\n");
		exit(1); ;
	}
	vars->current_token[0] = '\0';
	vars->is_s_quote = 0;
	vars->is_d_quote = 0;
	vars->is_redirection = 0;
	vars->is_pipe = 0;
}

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
		return (process_redirection(vars, token_lst, line, i, TOKEN_RED_IN, 1));
	if (line[*i] == '>')
		return (process_redirection(vars, token_lst, line, i, TOKEN_RED_OUT, 1));
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
