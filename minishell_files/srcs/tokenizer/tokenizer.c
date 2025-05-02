/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:06:19 by opopov            #+#    #+#             */
/*   Updated: 2025/05/02 11:04:15 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_quotes(t_tokenize_struct *vars, char *line, t_token_lst **token_lst)
{
	int	i;
	int	temp;

	i = 0;
	if (line[i] == '\"')
	{
		temp = i;
		temp += copy_token_till_delimiter(&vars->current_token,
				line + i, '\"', token_lst);
		if (temp < i)
			return (-1);
		i = temp;
	}
	if (line[i] == '\'')
	{
		temp = i;
		temp += copy_token_till_delimiter(&vars->current_token,
				line + i, '\'', token_lst);
		if (temp < i)
			return (-1);
		i = temp;
	}
	return (i);
}

int	handle_other_tokens(char *line, t_token_lst **token_lst,
		t_tokenize_struct *vars)
{
	int	i;

	i = 0;
	if (!line || line[0] == '\0')
		return (0);
	if (line[i] == '<' && line[i + 1] == '<')
		return (send_str_to_token_lst("<<", token_lst, TOKEN_HEREDOC));
	if (line[i] == '>' && line[i + 1] == '>')
		return (send_str_to_token_lst(">>", token_lst, TOKEN_APPEND));
	if (line[i] == '&' && line[i + 1] == '&')
		return (send_str_to_token_lst("&&", token_lst, TOKEN_AND));
	if (line[i] == '|' && line[i + 1] == '|')
		return (send_str_to_token_lst("||", token_lst, TOKEN_OR));
	if (line[i] == '<')
		return (send_str_to_token_lst("<", token_lst, TOKEN_REDIRECTION_IN));
	if (line[i] == '>')
		return (send_str_to_token_lst(">", token_lst, TOKEN_REDIRECTION_OUT));
	if (line[i] == '|')
		return (send_str_to_token_lst("|", token_lst, TOKEN_PIPE));
	if (line[i] == '(')
		return (send_paren_to_token_lst("(", token_lst, TOKEN_L_PAREN, vars));
	if (line[i] == ')')
		return (send_paren_to_token_lst(")", token_lst, TOKEN_R_PAREN, vars));
	return (0);
}

int	append_to_token(t_token_type type, char *current_token,
		t_token_lst **token_lst)
{
	char		*temp;
	t_token_lst	*temp_lst;

	temp = ft_strdup(current_token);
	if (!temp)
	{
		ft_putstr_fd("Error: failed allocation memory", 2);
		return (-1);
	}
	temp_lst = token_new_node(type, temp);
	if (!temp_lst)
	{
		free(temp);
		ft_putstr_fd("Error: failed allocation memory", 2);
		return (-1);
	}
	token_add_node_back(token_lst, temp_lst);
	current_token[0] = '\0';
	return (0);
}

int	handle_wildcard(char *current_token, char *line, t_token_lst **token_lst)
{
	int			i;
	t_token_lst	*temp_lst;

	i = 0;
	if (is_wildcard_present(line))
	{
		temp_lst = wildcard_function(line, &i);
		if (temp_lst)
		{
			token_add_node_back(token_lst, temp_lst);
		}
		else
		{
			i = copy_until_special_char(current_token, line, "<>|&()\"\'");
			if (current_token[0] != '\0')
			{
				if (append_to_token(TOKEN_WORD, current_token, token_lst) == -1)
					return (-1);
			}
		}
	}
	return (i);
}

t_token_lst	*ft_tokenize(char *line)
{
	t_token_lst			*token_lst;
	t_tokenize_struct	vars;
	int					i;

	initialize_tokenize_struct(&vars, line);
	token_lst = NULL;
	i = 0;
	while (line[i])
	{
		if (!ft_tokenize_loop_part1(line, &i, &vars, &token_lst))
			return (NULL);
		if (line[i] == '\0')
			break ;
		if (!ft_tokenize_loop_part2(line, &i, &vars, &token_lst))
			return (NULL);
	}
	free(vars.current_token);
	if (vars.paren_counter != 0)
	{
		ft_putstr_fd("Error: unmatched parenthesis\n", 2);
		token_free_list(token_lst);
		return (NULL);
	}
	token_add_node_back(&token_lst, token_new_node(TOKEN_END, NULL));
	return (token_lst);
}
