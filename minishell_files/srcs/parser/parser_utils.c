#include "../../includes/minishell.h"

char	*dollar_execute(char *name)
{
	char	*res;

	if (!name)
		return (NULL);
	res = getenv(name);
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

char *ft_strjoin_char(const char *s1, char c)
{
	int		len;
	char	*str;

	len = (int)ft_strlen(s1);
	str = (char *)malloc(len + 2);
	if (!str)
		return NULL;
	ft_strlcpy(str, s1, len + 1);
	str[len] = c;
	str[len + 1] = '\0';
	return str;
}

char	*dollar_check(char *value, int *i)
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
	tmp = dollar_execute(name);
	if (!tmp)
	{
		(*i) += name_len + 1;
		return (ft_strdup(""));
	}
	new = ft_strdup(tmp);
	// free(tmp);
	*i += name_len + 1;
	// free(name);
	return (new);
}

char *arg_word_return(char *value)
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
			tmp = dollar_check(value, &i);
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

char	*arg_d_quote_return(char *value)
{
	char	*res;

	if (!value)
		return (NULL);
	res = ft_strtrim(value, "\"");
	res = arg_word_return(res);
	return (res);
}

char	*arg_return(char *value, t_token_type type)
{
	if (!value)
		return (NULL);
	if (type == TOKEN_D_QUOTE)
		return (arg_d_quote_return(value));
	if (type == TOKEN_S_QUOTE)
		return (ft_strtrim(value, "\'"));
	if (type == TOKEN_ENV_VAR || type == TOKEN_WORD)
		return (arg_word_return(value));
	return (ft_strdup(value));
}
