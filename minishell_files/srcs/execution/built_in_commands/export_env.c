/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:19:25 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 16:36:59 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/built_in_commands.h"

void	ft_sort_env(char **env, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	ft_count_env(t_shell shell)
{
	int	i;

	i = 0;
	while (shell.env[i])
		i++;
	return (i);
}

char	**ft_copy_env(t_shell shell)
{
	int		i;
	char	**res;
	int		count;

	count = ft_count_env(shell);
	res = (char **)malloc(sizeof(char *) * (count + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (shell.env[i])
	{
		res[i] = ft_strdup(shell.env[i]);
		if (!res[i])
		{
			while (i-- > 0)
				free(res[i]);
			free(res);
			return (NULL);
		}
		i++;
	}
	res[i] = NULL;
	ft_sort_env(res, i);
	return (res);
}

void	ft_export_env_write_value(char *value, char **copy, int i)
{
	if (value)
	{
		write(1, copy[i], value - copy[i]);
		write(1, "=\"", 2);
		write(1, value + 1, ft_strlen(value + 1));
		write(1, "\"\n", 2);
	}
	else
	{
		write(1, copy[i], ft_strlen(copy[i]));
		write(1, "\n", 1);
	}
}

void	ft_export_env(t_shell shell)
{
	char	**copy;
	int		i;
	char	*value;

	copy = ft_copy_env(shell);
	if (!copy)
		return ;
	i = 0;
	while (copy[i])
	{
		if (!ft_strncmp(copy[i], "_=", 2))
		{
			i++;
			continue ;
		}
		write(1, "declare -x ", 11);
		value = ft_strchr(copy[i], '=');
		ft_export_env_write_value(value, copy, i);
		i++;
	}
	env_free(copy);
}
