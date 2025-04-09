/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:43:18 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/09 12:51:52 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"



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

// int	handle_whitespace(t_tokenize_struct *vars, t_token_lst **token_lst, char *line, int *i)
// {
// 	char	*temp;

// 	if (ft_isblank(line[*i]))
// 	{
// 		while (ft_isblank(line[*i]))
// 			(*i)++;
// 		if (vars->current_token[0] == '\0')
// 			return (1);
// 		temp = ft_strdup(vars->current_token); // check if not NULL?
// 		token_add_node_back(token_lst, token_new_node(0, temp));
// 		vars->current_token[0] = '\0';
// 		return (1);
// 	}
// 	return (0);
// }

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

t_token_type return_token_type(char *str)
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

int	copy_token_till_delimiter(char **dest, char *src, char delimiter, t_token_lst **token_lst)
{
	int	i;
	int	j;
	char *temp;
	t_token_type	temp_token;
	j = 0;
	i = 1; // skip the first quote
	while ((*dest)[j])
		j++;
	while (src[i] && (src[i] != delimiter))
	{
		(*dest)[j] = src[i];
		j++;
		i++;
		if (src[i] && (src[i] == delimiter && src[i + 1] == delimiter)) // to handle this case cat "hell""$PATH"
			i+= 2;
	}
	(*dest)[j] = '\0';
	if (src[i] == delimiter)
	{
		temp = ft_strdup(*dest); // if malloc fails???? (free everything, report failure and return)
		**dest = '\0';
		temp_token = return_token_type(src + i);
		token_add_node_back(token_lst, token_new_node(temp_token, temp));
		i++;
		return (i);
	}
	free (*dest);
	token_free_list(*token_lst);
	ft_putstr_fd("Error: unmatched quotes\n", 2);
	exit(1);
}

int	handle_quotes(t_tokenize_struct *vars, char *line, t_token_lst **token_lst)
{
	int	i;

	i = 0;
	if (line[i] == '\"')
		i += copy_token_till_delimiter(&vars->current_token, line + i, '\"', token_lst);
	if (line[i] == '\'')
		i += copy_token_till_delimiter(&vars->current_token, line + i, '\'', token_lst);
	return (i);
}

// int	handle_redirection(t_tokenize_struct *vars, t_token_lst **token_lst, char *line, int *i)
// {
// 	if (line[*i] == '<')
// 		return (process_redirection(vars, token_lst, line, i, TOKEN_REDIRECTION_IN , 1));
// 	if (line[*i] == '>')
// 		return (process_redirection(vars, token_lst, line, i, TOKEN_REDIRECTION_OUT , 1));
// 	if (line[*i] == '>' && line[*i + 1] == '>')
// 		return (process_redirection(vars, token_lst, line, i, TOKEN_APPEND, 2));
// 	if (line[*i] == '|')
// 		return (process_redirection(vars, token_lst, line, i, TOKEN_PIPE, 1));
// 	return (0);
// }

int send_str_to_token_lst(char *str, t_token_lst **token_lst, t_token_type token_type)
{
	char	*temp;

	if (str[0] != '\0')
	{
		temp = ft_strdup(str);
		token_add_node_back(token_lst, token_new_node(token_type, temp));
	}
	return (ft_strlen(str));
}

int handle_other_tokens(char *line, t_token_lst **token_lst)
{
	int	i;

	i = 0;
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
		return (send_str_to_token_lst("(", token_lst, TOKEN_L_PAREN));
	if (line[i] == ')')
		return (send_str_to_token_lst(")", token_lst, TOKEN_R_PAREN));
	if (line[i] == '$')
		return (send_str_to_token_lst("$", token_lst, TOKEN_ENV_VAR));
	return (0);
}

int	create_word_token(char *current_token, char *line, t_token_lst **token_lst)
{
	int		i;
	char	*temp;

	i = 0;
	while (line[i] && !ft_isblank(line[i]) && ft_strchr("<>|&()$\"\'", line[i]) == NULL)
	{
		ft_append_char(current_token, line[i]); // change ft_append_char
		i++;
	}
	if (current_token[0] != '\0')
	{
		temp = ft_strdup(current_token);
		token_add_node_back(token_lst, token_new_node(0, temp));
		current_token[0] = '\0';
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
		while (ft_isblank(line[i]))
			i++;
		i += handle_quotes(&vars, line + i, &token_lst);
		if (line[i] == '\0')
			break ;
		i += handle_other_tokens(line + i, &token_lst);
		while (ft_isblank(line[i]))
			i++;
		i+= create_word_token(vars.current_token, line + i, &token_lst);
		// if none of the chars are special char copy the string until next space as a word token
	}
	return (token_lst);
}
