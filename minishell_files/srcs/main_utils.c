/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:32:43 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 14:38:18 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**create_copy_env(char **envp, int count, int *j)
{
	char	**copy;

	if (!search_shlvl(envp))
	{
		copy = malloc(sizeof(char *) * (count + 2));
		if (!copy)
			return (NULL);
		copy[(*j)++] = ft_strdup("SHLVL=1");
	}
	else
	{
		copy = malloc(sizeof(char *) * (count + 1));
		if (!copy)
			return (NULL);
	}
	return (copy);
}

void	increment_shlvl(char **envp, int i, int *j, char **copy)
{
	int		curr_shlvl;
	char	*tmp;

	curr_shlvl = ft_atoi(envp[i] + 6);
	if (curr_shlvl >= 999)
	{
		printf("warning: shell level (%d) too high, resetting to 1\n",
			curr_shlvl + 1);
		copy[(*j)++] = ft_strdup("SHLVL=1");
	}
	else
	{
		tmp = ft_itoa(curr_shlvl + 1);
		copy[(*j)++] = ft_strjoin("SHLVL=", tmp);
		free(tmp);
	}
}

char	**copy_env(char **envp)
{
	int		count;
	char	**copy;
	int		i;
	int		j;

	count = 0;
	i = 0;
	j = 0;
	while (envp[count])
		count++;
	copy = create_copy_env(envp, count, &j);
	if (!copy)
		return (NULL);
	while (i < count)
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			increment_shlvl(envp, i, &j, copy);
		else
			copy[j++] = ft_strdup(envp[i]);
		i++;
	}
	copy[j] = NULL;
	return (copy);
}
