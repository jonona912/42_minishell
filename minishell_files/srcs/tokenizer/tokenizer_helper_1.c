#include "../../includes/minishell.h"

int	ft_isblank(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int	ft_append_char(char *str, char c)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	str[i] = c;
	str[i + 1] = '\0';
	return (1);
}

int	handle_unmatched_quotes(t_tokenize_struct *vars, t_token_lst **token_lst)
{
	if (vars->current_token[0] != '\0')
	{
		char *temp = ft_strdup(vars->current_token);
		token_add_node_back(token_lst, token_new_node(0, temp));
	}
	free(vars->current_token);
	if (vars->is_s_quote || vars->is_d_quote)
	{
		printf("Error: unmatched quotes\n");
		token_free_list(*token_lst);
		return (1);
	}
	return (0);
}

int	process_redirection(t_tokenize_struct *vars, t_token_lst **token_lst, char *line, int *i, t_token_type token_type, int step)
{
	char	*temp;

	if (vars->current_token[0] != '\0')
	{
		temp = ft_strdup(vars->current_token);
		token_add_node_back(token_lst, token_new_node(0, temp));
		vars->current_token[0] = '\0';
	}
	ft_append_char(vars->current_token, line[*i]);
	if (step == 2)
		ft_append_char(vars->current_token, line[*i + 1]);
	temp = ft_strdup(vars->current_token);
	token_add_node_back(token_lst, token_new_node(token_type, temp));
	vars->current_token[0] = '\0';
	*i += step;
	return (1);
}

int	initialize_tokenize_struct(t_tokenize_struct *vars, char *line)
{
	int	line_len;

	line_len = strlen(line);
	vars->current_token = (char *)malloc((line_len + 1) * sizeof(char));
	if (!vars->current_token)
	{
		printf("Error: memory allocation failed\n");
		return (-1);
	}
	vars->current_token[0] = '\0';
	vars->is_parenthesis = 0;
	vars->paren_counter = 0;
	return (0);
}
