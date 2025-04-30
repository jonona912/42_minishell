#include "../../includes/minishell.h"

int	return_executable_path_loop(char **path_split, const char *name,
		char **temp_path)
{
	int		i;

	i = 0;
	while (path_split[i])
	{
		*temp_path = ft_strjoin_three(path_split[i], "/", name);
		if (*temp_path == NULL)
			return (0);
		if (access(*temp_path, X_OK) == 0)
			return (1);
		free(*temp_path);
		*temp_path = NULL;
		i++;
	}
	return (0);
}

int	copy_wildcard_string_loop(char *line, int i, char **dest)
{
	int	len;

	len = 0;
	while (is_not_special_char(line[i]) && line[i] != '*')
	{
		(*dest)[len] = line[i];
		i++;
		len++;
	}
	while (line[i] && line[i] == '*')
		i++;
	(*dest)[len] = '\0';
	return (i);
}

char	*return_executable_path(const char *name, t_shell *shell)
{
	char	*path;
	char	**path_split;
	char	*temp_path;
	int		ch;

	temp_path = NULL;
	if (name == NULL)
		return (NULL);
	if (name[0] == '/' || name[0] == '.')
	{
		if (access(name, X_OK) == 0)
			return (ft_strdup((char *) name));
		return (NULL);
	}
	path = ft_getenv("PATH", *shell);
	if (path == NULL)
		return (ft_strdup((char *) name));
	path_split = ft_split(path, ':');
	if (path_split == NULL)
	{
		ft_free_double_ptr(path_split);
		return (ft_strdup((char *) name));
	}
	ch = return_executable_path_loop(path_split, name, &temp_path); // delete ch
	ft_free_double_ptr(path_split);
	// if (!ch) // ch == 0?
	// 	return (NULL);
	if (!temp_path)
		temp_path = ft_strdup((char *) name);
	return (temp_path);
}
