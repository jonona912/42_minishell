#include "../includes/minishell.h"

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
	while(str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	**copy_env(char **envp)
{
	int		count;
	char	**copy;
	int		i;
	int		j;
	int		curr_shlvl;
	char	*tmp;

	count = 0;
	while (envp[count])
		count++;
	i = 0;
	j = 0;
	if (!search_shlvl(envp))
	{
		copy = malloc(sizeof(char *) * (count + 2));
		if (!copy)
			return (NULL);
		copy[j++] = ft_strdup("SHLVL=1");
	}
	else
	{
		copy = malloc(sizeof(char *) * (count + 1));
		if (!copy)
			return (NULL);
	}
	while (i < count)
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			curr_shlvl = ft_atoi(envp[i] + 6);
			if (curr_shlvl >= 999)
			{
				printf("warning: shell level (%d) too high, resetting to 1\n", curr_shlvl + 1);
				copy[j++] = ft_strdup("SHLVL=1");
			}
			else
			{
				tmp = ft_itoa(curr_shlvl + 1);
				copy[j++] = ft_strjoin("SHLVL=", tmp);
				free(tmp);
			}
		}
		else
			copy[j++] = ft_strdup(envp[i]);
		i++;
	}
	copy[j] = NULL;
	return (copy);
}
