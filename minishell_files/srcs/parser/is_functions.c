#include "../../includes/minishell.h"

int	is_cmd_valid(t_token_type type)
{
	return (type == TOKEN_WORD
		|| is_redirection(type)
		|| type == TOKEN_ENV_VAR
		|| type == TOKEN_D_QUOTE
		|| type == TOKEN_S_QUOTE);
}

int	is_token_valid_for_cmd(t_token_type type)
{
	return (type == TOKEN_WORD || type == TOKEN_ENV_VAR
		|| type == TOKEN_D_QUOTE || type == TOKEN_S_QUOTE);
}
