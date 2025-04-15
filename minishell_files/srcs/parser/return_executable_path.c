#include "../../includes/minishell.h"

char *return_executable_path(const char *name)
{
	char	*path;
	char	**path_split;
	char	*temp_path;
	int		i;

	i = 0;
	if (name == NULL)
		return NULL;
	if (name[0] == '/' || name[0] == '.') // test this feature
	{
		if (access(name, X_OK) == 0)
			return ft_strdup((char *) name);
		return NULL;
	}
	path = getenv("PATH");
	if (path == NULL)
		return NULL;
	path_split = ft_split(path, ':');
	if (path_split == NULL)
		return NULL;
	while (path_split[i])
	{
		temp_path = ft_strjoin_three(path_split[i], "/", name);
		if (temp_path == NULL)
		{
			ft_free_double_ptr(path_split);
			return NULL;
		}
		if (access(temp_path, X_OK) == 0)
		{
			ft_free_double_ptr(path_split);
			return temp_path;
		}
		free(temp_path);
		i++;
	}
	ft_free_double_ptr(path_split);
	return (NULL);
}
