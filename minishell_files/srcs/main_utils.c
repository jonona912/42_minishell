#include "../includes/minishell.h"

int	search_my_shlvl(t_shell shell)
{
	int	i;

	i = 0;
	while (shell.env[i])
	{
		if (ft_strncmp(shell.env[i], "MY_SHLVL=", 9) == 0)
			return (ft_atoi(shell.env[i] + 9));
		i++;
	}
	return (1);
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
	int		is_shlvl;
	int		curr_shlvl;

	count = 0;
	while (envp[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	is_shlvl = 0;
	while (i < count)
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			is_shlvl = 1;
			curr_shlvl = ft_atoi(envp[i] + 6);
			if (curr_shlvl >= 999)
			{
				printf("Error: SHLVL is too high\n");
				env_free(copy);
				exit(1);
			}
			copy[i] = ft_strjoin("SHLVL=", ft_itoa(curr_shlvl + 1));
		}
		else
			copy[i] = ft_strdup(envp[i]);
		i++;
	}
	if (!is_shlvl)
		copy[i++] = ft_strdup("SHLVL=1");
	copy[i] = NULL;
	return (copy);
}
