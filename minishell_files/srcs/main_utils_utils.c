/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:33:06 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 13:36:39 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_first_token_valid(t_token_type type)
{
	return (type == TOKEN_WORD
		|| type == TOKEN_REDIRECTION_IN
		|| type == TOKEN_REDIRECTION_OUT
		|| type == TOKEN_APPEND
		|| type == TOKEN_HEREDOC
		|| type == TOKEN_L_PAREN
		|| type == TOKEN_R_PAREN);
}

int	search_shlvl(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	env_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}
