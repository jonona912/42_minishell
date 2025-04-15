/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:43:18 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/15 17:37:30 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

// changing this here 
int	copy_token_till_delimiter(char **dest, char *src, char delimiter, t_token_lst **token_lst)
{
	int	i;
	int	j;
	char *temp;
	t_token_lst	*temp_lst;
	j = 0;
	i = 0; // skip the first quote
	while ((*dest)[j])
		j++;
	(*dest)[j++] = src[i++];
	while (src[i] && (src[i] != delimiter))
	{
		(*dest)[j++] = src[i++];
		if (src[i] && (src[i] == delimiter && src[i + 1] == delimiter)) // to handle this case cat "hell""$PATH"
			i+= 2;

	}
	(*dest)[j] = '\0';
	if (src[i] == delimiter)
	{
		(*dest)[j] = delimiter;
		(*dest)[++j] = '\0';
		temp = ft_strdup(*dest); // if malloc fails???? (free everything, report failure and return)
		if (!temp)
		{
			ft_putstr_fd("Error: failed allocation memory", 2);
			return (-1); // change code later (redacted)
		}
		**dest = '\0';
		temp_lst = token_new_node(return_token_type(src + i), temp);
		if (!temp_lst)
		{
			free(temp);
			ft_putstr_fd("Error: failed allocation memory", 2);
			return (-1); //change code later
		}
		token_add_node_back(token_lst, temp_lst);
		return (++i);
	}
	ft_putstr_fd("Error: unmatched quotes\n", 2);
	return (-1);
}

int	handle_quotes(t_tokenize_struct *vars, char *line, t_token_lst **token_lst)
{
	int	i;
	int	temp;

	i = 0;
	if (line[i] == '\"')
	{
		temp = i;
		temp += copy_token_till_delimiter(&vars->current_token, line + i, '\"', token_lst);
		if (temp < i)
			return (-1);
		i = temp;
	}
	if (line[i] == '\'')
	{
		temp = i;
		temp += copy_token_till_delimiter(&vars->current_token, line + i, '\'', token_lst);
		if (temp < i)
			return (-1);
		i = temp;
	}
	return (i);
}

int	send_paren_to_token_lst(char *str, t_token_lst **token_lst, t_token_type token_type, t_tokenize_struct *vars)
{
	char		*temp;
	t_token_lst	*temp_lst;

	temp_lst = NULL;
	temp = ft_strdup(str);
	if (!temp)
	{
		ft_putstr_fd("Error: failed allocation memory", 2);
		return (-1); // change code later (redacted)
	}
	if (str[0] == '(')
		vars->paren_counter++;
	else if (str[0] == ')')
	{
		if (vars->paren_counter == 0)
		{
			free(temp);
			ft_putstr_fd("Error: unmatched parenthesis", 2);
			return (-1); // change code later (redacted)
		}
		vars->paren_counter--;
	}
	temp_lst = token_new_node(token_type, temp);
	if (!temp_lst)
	{
		free(temp);
		ft_putstr_fd("Error: failed allocation memory", 2);
		return (-1); //change code later
	}
	token_add_node_back(token_lst, temp_lst);
	return (1);
}

int send_str_to_token_lst(char *str, t_token_lst **token_lst, t_token_type token_type)
{
	char	*temp;

	if (str[0] != '\0')
	{
		temp = ft_strdup(str);
		if (!temp)
		{
			ft_putstr_fd("Error: failed allocation memory", 2);
			return (-1); // change code later (redacted)
		}
		token_add_node_back(token_lst, token_new_node(token_type, temp));
	}
	return (ft_strlen(str));
}

int handle_other_tokens(char *line, t_token_lst **token_lst, t_tokenize_struct *vars)
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
		return (send_paren_to_token_lst("(", token_lst, TOKEN_L_PAREN, vars));
	if (line[i] == ')')
		return (send_paren_to_token_lst(")", token_lst, TOKEN_R_PAREN, vars));
	if (line[i] == '$')
		return (send_str_to_token_lst("$", token_lst, TOKEN_ENV_VAR));
	return (0);
}

int	create_word_token(char *current_token, char *line, t_token_lst **token_lst)
{
	int		i;
	char	*temp;
	t_token_lst	*temp_lst;

	i = 0;
	temp_lst = NULL;
	while (line[i] && !ft_isblank(line[i]) && ft_strchr("<>|&()$\"\'", line[i]) == NULL)
	{
		ft_append_char(current_token, line[i]); // change ft_append_char
		i++;
	}
	if (current_token[0] != '\0')
	{
		temp = ft_strdup(current_token);
		if (!temp)
		{
			ft_putstr_fd("Error: failed allocation memory", 2);
			return (-1); // change code later
		}
		temp_lst = token_new_node(0, temp);
		if (!temp_lst)
		{
			free(temp);
			ft_putstr_fd("Error: failed allocation memory", 2);
			return (-1); // change code later
		}
		token_add_node_back(token_lst, temp_lst);
		current_token[0] = '\0';
	}
	return (i);
}

t_token_lst	*ft_tokenize(char *line)
{
	t_token_lst			*token_lst;
	t_tokenize_struct	vars;
	int					i;
	int					temp;

	initialize_tokenize_struct(&vars, line);
	token_lst = NULL;
	i = 0;
	while (line[i])
	{
		while (ft_isblank(line[i]))
			i++;
		temp = handle_quotes(&vars, line + i, &token_lst);
		if (temp < 0)
		{
			free(vars.current_token);
			token_free_list(token_lst);
			return (NULL);
		}
		i += temp;
		if (line[i] == '\0')
			break ;
		temp = handle_other_tokens(line + i, &token_lst, &vars);
		if (temp < 0)
		{
			free(vars.current_token);
			token_free_list(token_lst);
			return (NULL);
		}
		i += temp;
		while (ft_isblank(line[i]))
			i++;
		temp = create_word_token(vars.current_token, line + i, &token_lst);
		if (temp < 0)
		{
			token_free_list(token_lst);
			free(vars.current_token);
			return (NULL);
		}
		i += temp;
		// if none of the chars are special char copy the string until next space as a word token
	}
	free(vars.current_token);
	if (vars.paren_counter != 0)
	{
		ft_putstr_fd("Error: unmatched parenthesis", 2);
		token_free_list(token_lst);
		return (NULL); // change code later
	}
	token_add_node_back(&token_lst, token_new_node(TOKEN_END, NULL)); // add this to the end of the list
	return (token_lst);
}
