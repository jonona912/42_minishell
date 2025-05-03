#include "../../includes/minishell.h"

char	*dollar_check(char *value, int *i, t_shell shell)
{
	char	*name;
	char	*tmp;
	int		name_len;

	name = name_finder(value, *i);
	if (!name)
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	name_len = ft_strlen(name);
	tmp = dollar_execute(name, shell);
	free(name);
	if (!tmp)
	{
		(*i) += name_len + 1;
		return (ft_strdup(""));
	}
	*i += name_len + 1;
	return (tmp);
}

int	dollar_structure_if(char *value, int *i, t_shell shell, char **res)
{
	char	*new;
	char	*tmp;

	if (value[*i] == '$' && (value[*i + 1] || value[*i + 1] == '?'))
	{
		if (value[*i] == '$' && value[*i + 1] == '?')
		{
			new = ft_strjoin(*res, ft_itoa(shell.last_status));
			free(*res);
			*res = new;
			*i += 2;
			return (1);
		}
		if (!ft_isspace(value[*i + 1]))
		{
			tmp = dollar_check(value, i, shell);
			new = ft_strjoin(*res, tmp);
			free(*res);
			free(tmp);
			*res = new;
			return (1);
		}
	}
	return (0);
}

char	*arg_word_return(char *value, t_shell shell)
{
	int		i;
	char	*res;
	char	*tmp;

	if (!value)
		return (NULL);
	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (value[i])
	{
		if (dollar_structure_if(value, &i, shell, &res))
			continue ;
		tmp = ft_strjoin_char(res, value[i]);
		free(res);
		res = tmp;
		i++;
	}
	// printf("DEBUG (words): %s\n", res);
	return (res);
}

char	*arg_d_quote_return(char *value, t_shell shell)
{
	char	*res;
	char	*tmp;

	if (!value)
		return (NULL);
	// printf("DEBUG (quotes): %s\n", value);
	tmp = ft_strtrim(value, "\"");
	if (!tmp)
		return (NULL);
	res = arg_word_return(tmp, shell);
	free(tmp);
	return (res);
}

char	*arg_return(char *value, t_token_type type, t_shell *shell)
{
	if (!value)
		return (NULL);
	if (type == TOKEN_D_QUOTE)
	{
		// printf("DEBUG (TOKEN_D_QUOTE): %s\n", value);
		return (arg_d_quote_return(value, *shell));
	}
	if (type == TOKEN_S_QUOTE)
	{
		// printf("DEBUG (TOKEN_S_QUOTE): %s\n", value);
		return (ft_strtrim(value, "\'"));
	}
	if (type == TOKEN_ENV_VAR || type == TOKEN_WORD)
	{
		// printf("DEBUG (TOKEN_WORD): %s\n", value);
		return (arg_word_return(value, *shell));
	}
	return (ft_strdup(value));
}
