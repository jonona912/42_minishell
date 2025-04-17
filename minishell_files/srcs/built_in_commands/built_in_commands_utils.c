#include "../../includes/minishell.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// extern char **environ;

int ft_putenv(char *str, t_shell *shell) // change here
{
	char	*eq_str;
	int		name_len;
	int		i;
	int		env_y;
	char	**new_env;

	if (!str || !ft_strchr(str, '='))
		return (1);
	eq_str = ft_strchr(str, '=');
	name_len = eq_str - str;
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], str, name_len) == 0 && shell->env[i][name_len] == '=')
		{
			shell->env[i] = str;
			return (0);
		}
		i++;
	}
	env_y = 0;
	while (shell->env[env_y]) env_y++;
	new_env = (char **) malloc(sizeof(char *) * (env_y + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (i < env_y)
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[env_y] = str;
	new_env[env_y + 1] = NULL;
	shell->env = new_env; // Point to the new array
	return 0;
}


int	ft_setenv(char *name, char *value, int overwrite, t_shell *shell) // change here
{
	char	*current_value;
	char	*tmp;
	int		len;

	tmp = NULL;
	if (!name || !value || ft_strchr(name, '='))
		return (1);
	current_value = getenv(name);
	if (current_value && !overwrite)
		return (0);
	len = ft_strlen(name) + ft_strlen(value) + 2;
	tmp = malloc(len);
	if (!tmp)
	{
		perror("Error: Failed allocation memory");
		return (1);
	}
	ft_strlcpy(tmp, name, len);
	ft_strlcat(tmp, "=", len);
	ft_strlcat(tmp, value, len);
	if (ft_putenv(tmp, shell))
	{
		free(tmp);
		perror("Error: putenv failed");
		return (1);
	}
	return (0);
}
