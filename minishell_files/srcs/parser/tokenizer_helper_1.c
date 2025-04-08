/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:16:52 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/08 12:19:48 by zkhojazo         ###   ########.fr       */
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

void	initialize_tokenize_struct(t_tokenize_struct *vars, char *line) // should return if fail or exit?
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
	vars->is_parenthesis = 0;
}
