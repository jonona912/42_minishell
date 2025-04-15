/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:35:38 by opopov            #+#    #+#             */
/*   Updated: 2025/04/15 09:54:32 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// char	*dollar_token(char *input, int exit_status)
// {
// 	char	*res;
// 	int		i;

// 	if (!input)
// 		return (NULL);
// 	while (input[i])
// 	{

// 		i++;
// 	}
// }

char *ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*res;
	if (!s1 || !s2 || !s3)
		return (NULL);

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

int	ft_setenv(char *name, char *value, int overwrite)
{
	extern char	**environ;
	int			name_len;
	int			value_len;
	int			i;
	char		*tmp;
	int			env_y;
	char		**env_2;

	if (!name || !value || ft_strchr(name, '=') != NULL)
	{
		ft_putstr_fd("Error: invalid syntax for setenv\n", 2);
		return (1);
	}
	name_len = ft_strlen(name);
	value_len = ft_strlen(value);

	// search for existing variable
	i = 0;
	while (environ[i])
	{
		if (strncmp(environ[i], name, name_len) == 0 &&
			environ[i][name_len] == '=')
		{
			if (!overwrite)
				return 0;
			tmp = ft_strjoin3(name, "=", value);
			if (!tmp)
			{
				ft_putstr_fd("Error: failed allocation memory\n", 2);
				return (1);
			}
			free(environ[i]);
			environ[i] = tmp;
			return (0);
		}
		i++;
	}

	// add new variable
	env_y = 0;
	while (environ[env_y])
		env_y++;
	env_2 = (char **)malloc((env_y + 2) * sizeof(char *));
	if (!env_2)
	{
		ft_putstr_fd("Error: failed allocation memory\n", 2);
		return (1);
	}
	i = 0;
	// copy variables
	while (i < env_y)
	{
		env_2[i] = environ[i];
		i++;
	}
	tmp = ft_strjoin3(name, "=", value);
	if (!tmp)
	{
		free(env_2);
		ft_putstr_fd("Error: failed allocation memory\n", 2);
		return (1);
	}
	env_2[env_y] = tmp;
	env_2[env_y + 1] = NULL;
	environ = env_2;
	return (0);
}
