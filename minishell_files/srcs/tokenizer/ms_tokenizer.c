/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:43:18 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/01 12:14:37 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	vars->is_redirection = 0;
	vars->is_pipe = 0;
}

t_token_lst	*ft_tokenize(char *line)
{
	t_token_lst			*token_lst;
	t_tokenize_struct	vars;
	char				*temp;
	int					i;

	initialize_tokenize_struct(&vars, line);
	token_lst = NULL;
	i = 0;
	while (ft_isspace(line[i]))
		i++;
	while (line[i])
	{
		if (line[i] == '\"')
		{
			vars.is_d_quote = !vars.is_d_quote;
			ft_append_char(vars.current_token, line[i]);
			i++;
			continue ;
		}
		if (line[i] == '\'')
		{
			vars.is_s_quote = !vars.is_s_quote;
			ft_append_char(vars.current_token, line[i]);
			i++;
			continue ;
		}
		if (line[i] == ' ' && !vars.is_s_quote && !vars.is_d_quote)
		{
			while (ft_isspace(line[i]))
				i++;
			temp = ft_strdup(vars.current_token); // check if not NULL?
			ms_token_add_node_back(&token_lst, ms_token_new_node(temp));
			vars.current_token[0] = '\0';
			continue ;
		}
		if (line[i] == '|' && !vars.is_s_quote && !vars.is_d_quote)
		{
			ft_append_char(vars.current_token, line[i]);
			temp = ft_strdup(vars.current_token);
			ms_token_add_node_back(&token_lst, ms_token_new_node(temp));
			vars.current_token[0] = '\0';
			i++;
			continue ;
		}
		ft_append_char(vars.current_token, line[i]);
		i++;
	}
	if (vars.current_token[0] != '\0')
	{
		temp = ft_strdup(vars.current_token);
		ms_token_add_node_back(&token_lst, ms_token_new_node(temp));
	}
	free(vars.current_token);
	return (token_lst);
}
