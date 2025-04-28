#include "../../includes/minishell.h"

char	*ft_getenv(const char *name, t_shell shell)
{
	int		i;
	char	*equal;
	int		name_len;

	if (!name || !shell.env)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (shell.env[i])
	{
		equal = ft_strchr(shell.env[i], '=');
		if (equal && (equal - shell.env[i]) == name_len
			&& ft_strncmp(shell.env[i], name, name_len) == 0)
			return (equal + 1);
		i++;
	}
	return (NULL);
}

int	ft_putenv_end(t_shell *shell, char *str)
{
	int		i;
	int		env_y;
	char	**new_env;

	env_y = 0;
	while (shell->env[env_y])
		env_y++;
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
	free(shell->env);
	shell->env = new_env;
	return (0);
}

int	ft_putenv(char *str, t_shell *shell)
{
	char	*eq_str;
	int		name_len;
	int		i;

	if (!str || !ft_strchr(str, '='))
		return (1);
	eq_str = ft_strchr(str, '=');
	name_len = eq_str - str;
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], str, name_len) == 0
			&& shell->env[i][name_len] == '=')
		{
			free(shell->env[i]);
			shell->env[i] = str;
			return (0);
		}
		i++;
	}
	if (ft_putenv_end(shell, str))
		return (1);
	return (0);
}

int	ft_setenv(char *name, char *value, int overwrite, t_shell *shell)
{
	char	*current_value;
	char	*tmp;
	int		len;

	tmp = NULL;
	if (!name || !value || ft_strchr(name, '='))
		return (1);
	current_value = ft_getenv(name, *shell);
	if (current_value && !overwrite)
		return (0);
	len = ft_strlen(name) + ft_strlen(value) + 2;
	tmp = malloc(len);
	if (!tmp)
		return (perror("Error: Failed allocation memory"), 1);
	ft_strlcpy(tmp, name, len);
	ft_strlcat(tmp, "=", len);
	ft_strlcat(tmp, value, len);
	if (ft_putenv(tmp, shell))
	{
		free(tmp);
		return (perror("Error: putenv failed"), 1);
	}
	return (0);
}
