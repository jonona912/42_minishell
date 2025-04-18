#include "../../includes/minishell.h"

char	*dollar_execute(char *name, t_shell shell)
{
	char	*res;

	if (!name)
		return (NULL);
	res = ft_getenv(name, shell);
	if (!res)
		return (NULL);
	return (ft_strdup(res));
}

char	*name_finder(char *value, int pos)
{
	int		i;
	char	*name;

	i = 0;
	pos++;
	while (value[pos + i] && (ft_isalnum(value[pos + i]) || value[pos + i] == '_'))
		i++;
	if (i == 0)
		return (NULL);
	name = (char *)malloc(i + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, value + pos, i + 1);
	return (name);
}

char	*dollar_check(char *value, int *i, t_shell shell)
{
	char	*name;
	char	*tmp;
	char	*new;
	int		name_len;

	name = name_finder(value, *i); // value = $USER -> name = USER
	if (!name)
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	name_len = ft_strlen(name);
	tmp = dollar_execute(name, shell);
	if (!tmp)
	{
		// free(name);
		(*i) += name_len + 1;
		return (ft_strdup(""));
	}
	new = ft_strdup(tmp);
	// free(tmp);
	// free(name);
	*i += name_len + 1;
	return (new);
}

char *arg_word_return(char *value, t_shell shell)
{
	int		i;
	char	*res;
	char	*tmp;
	char	*new;

	if (!value)
		return (NULL);
	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (value[i])
	{
		if (value[i] == '$' && value[i + 1] && !ft_isspace(value[i + 1]))
		{
			tmp = dollar_check(value, &i, shell);
			new = ft_strjoin(res, tmp);
			// free(tmp);
			// free(res);
			res = new;
			continue;
		}
		tmp = ft_strjoin_char(res, value[i]);
		// free(res);
		res = tmp;
		i++;
	}
	return (res);
}

char	*arg_d_quote_return(char *value, t_shell shell)
{
	char	*res;
	char	*tmp;

	if (!value)
		return (NULL);
	tmp = ft_strtrim(value, "\"");
	res = arg_word_return(tmp, shell);
	free(tmp);
	return (res);
}

char	*arg_return(char *value, t_token_type type, t_shell shell)
{
	if (!value)
		return (NULL);
	if (type == TOKEN_D_QUOTE)
		return (arg_d_quote_return(value, shell));
	if (type == TOKEN_S_QUOTE)
		return (ft_strtrim(value, "\'"));
	if (type == TOKEN_ENV_VAR || type == TOKEN_WORD)
		return (arg_word_return(value, shell));
	return (ft_strdup(value));
}
