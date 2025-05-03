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

void	shell_env_free(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		free(shell->env[i]);
		i++;
	}
	free(shell->env);
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
	int		curr_shlvl;
	char	*tmp;

	count = 0;
	while (envp[count])
		count++;
	i = 0;
	if (!search_shlvl(envp))
	{
		copy = malloc(sizeof(char *) * (count + 2));
		if (!copy)
			return (NULL);
		copy[i++] = ft_strdup("SHLVL=1");
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
				printf("Error: SHLVL is too high\n");
				env_free(copy);
				exit(1);
			}
			tmp = ft_itoa(curr_shlvl + 1);
			copy[i] = ft_strjoin("SHLVL=", tmp);
			free(tmp);
		}
		else
			copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
