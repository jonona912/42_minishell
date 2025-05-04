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

int	ft_count_env_special(t_shell shell)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (shell.env[i])
	{
		if (ft_isalpha(shell.env[i][0]))
			count++;
		i++;
	}
	return (count);
}

char	**ft_copy_env(t_shell shell)
{
	int		i;
	char	**res;
	int		count;

	count = ft_count_env_special(shell);
	res = (char **)malloc(sizeof(char *) * (count + 1));
	if (!res)
		return (NULL);
	i = 0;
	count = 0;
	while (shell.env[i])
	{
		if (ft_isalpha(shell.env[i][0]))
			res[count++] = ft_strdup(shell.env[i]);
		i++;
	}
	res[count] = NULL;
	ft_sort_env(res, count);
	return (res);
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
		write(1, "declare -x ", 11);
		value = ft_strchr(copy[i], '=');
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
		i++;
	}
	env_free(copy);
}
