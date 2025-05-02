#include "../../includes/minishell.h"

int	check_cwd(t_shell *shell)
{
	char	cwd[4096];
	char	*old_pwd;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_setenv("PWD", cwd, 1, shell);
		return (0);
	}
	old_pwd = ft_getenv("OLDPWD", *shell);
	if (old_pwd)
	{
		ft_setenv("PWD", old_pwd, 1, shell);
		return (1);
	}
	return (0);
}

int	is_valid_name(char *name)
{
	int	i;

	i = 0;
	if (!name || !*name)
		return (0);
	if (!(ft_isalpha(name[i]) || name[i] == '_'))
		return (0);
	i++;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	ft_cd_end(char *curr_pwd, t_shell *shell, char **oldpwd, char *cwd)
{
	char	*oldpwd_tmp;

	if (*curr_pwd)
	{
		oldpwd_tmp = ft_strdup(curr_pwd);
		if (!oldpwd_tmp)
			return (printf("Error: allocation memory failed\n"), 1);
		if (ft_setenv("OLDPWD", oldpwd_tmp, 1, shell))
			return (free(oldpwd_tmp), 1);
	}
	check_cwd(shell);
	if (*oldpwd)
		free(*oldpwd);
	if (*curr_pwd)
		*oldpwd = ft_strdup(curr_pwd);
	else
		*oldpwd = ft_strdup(cwd);
	return (0);
}
