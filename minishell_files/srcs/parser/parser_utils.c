#include "../../includes/minishell.h"

char	*dollar_execute(char *name)
{
	char	*res;

	if (!name)
		return (NULL);
	// if (ft_strcmp(name, "?") == 0)
	// 	return (ft_itoa(exit_status));
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
	// if (value[pos] == '?')
	// {
	// 	name = (char *)malloc(2 * sizeof(char));
	// 	if (!name)
	// 		return (NULL);
	// 	name[0] = '?';
	// 	name[1] = '\0';
	// 	return (name);
	// }
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
	size_t len = ft_strlen(s1);
	char *str = malloc(len + 2);
	if (!str)
		return NULL;
	ft_strlcpy(str, s1, len + 1);
	str[len] = c;
	str[len + 1] = '\0';
	return str;
}

char *arg_word_return(char *value)
{
	int i = 0;
	char *res;
	char *tmp;
	char *tmp_name;
	char *var_value;

	res = ft_strdup("");
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
			var_value = dollar_execute(tmp_name);
			free(tmp_name);
			if (var_value)
			{
				tmp = ft_strjoin(res, var_value);
				free(res);
				res = tmp;
				free(var_value);
			}
			i += ft_strlen(tmp_name) + 1;
			continue;
		}
		tmp = ft_strjoin_char(res, value[i]);
		free(res);
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
	if (!res)
		return (NULL);
	return (res);
}

char	*arg_return(t_token_lst *lst)
{
	if (!lst || !lst->value)
		return (NULL);
	if (lst->type == TOKEN_D_QUOTE)
		return (arg_d_quote_return(lst->value));
	if (lst->type == TOKEN_S_QUOTE)
		return (ft_strtrim(lst->value, "\'"));
	if (lst->type == TOKEN_ENV_VAR)
		return (arg_word_return(lst->value));
	return (ft_strdup(lst->value));
}
