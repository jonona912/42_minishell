#include "../../includes/minishell.h"

char	*dollar_execute(char *name)
{
	char	*res;

	if (!name)
		return (NULL);
	res = getenv(name);
	free(name);
	if (!res)
		return (NULL);
	return (res);
}

char	*name_finder(char *value, int pos)
{
	int		i;
	char	*name;
	// char	*res;

	i = 0;
	pos++;
	while (value[pos + i] && (ft_isalnum(value[pos + i]) || value[pos + i] == '_'))
		i++;
	if (!i)
		return (NULL);
	name = (char *)malloc(i + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, value + pos, i + 1);
	// res = getenv(name);
	// free(name);
	// if (!res)
	// 	return (NULL);
	return (name);
}

int	dollar_len(char *value)
{
	int		i;
	char	*tmp;

	i = 0;
	while (value[i])
	{
		if (value[i] == '$' && value[i + 1])
		{
			tmp = getenv(value);
			i += ft_strlen(tmp);
			continue;
		}
		i++;
	}
	return (i);
}

char	*arg_word_return(char *value)
{
	int		value_len;
	int		i;
	int		j;
	char	*res;
	char	*tmp;
	char	*tmp_name;

	if (!value)
		return (NULL);
	value_len = dollar_len(value);
	res = (char *)malloc((value_len * sizeof(char)) + 1);
	i = 0;
	j = 0;
	while (value[i])
	{
		if (value[i] == '$' && value[i + 1])
		{
			tmp_name = name_finder(value, i);
			if (!tmp_name)
			{
				free(res);
				return (NULL);
			}
			tmp = dollar_execute(tmp_name);
			if (!tmp)
			{
				free(res);
				return (NULL);
			}
			ft_strjoin(res, tmp);
			j += ft_strlen(tmp);
			i += ft_strlen(tmp_name) + 1;
			free(tmp);
			continue;
		}
		res[j] = value[i];
		j++;
		i++;
	}
	res[j] = '\0';
	return (res);
}

char	*arg_d_quote_return(char *value)
{
	char	*tmp;
	char	*res;
	char	*quoted;

	if (!value)
		return (NULL);
	tmp = arg_word_return(value + 1);  // Skip opening quote
	if (!tmp)
		return (NULL);
	quoted = ft_strjoin("\"", tmp);
	res = ft_strjoin(quoted, "\"");
	free(tmp);
	free(quoted);
	return (res);
}


char	*arg_return(t_token_lst *lst)
{
	if (!lst || !lst->value)
		return (NULL);
	if (lst->type == TOKEN_D_QUOTE)
		return (arg_d_quote_return(lst->value));
	if (lst->type == TOKEN_WORD)
		return (arg_word_return(lst->value));
	return (ft_strdup(lst->value));
}
