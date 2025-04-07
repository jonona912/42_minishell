/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 14:01:59 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/06 19:55:46 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
void	ft_print_tokens(t_token_lst *token_lst);

int	main(void)
{
	t_token_lst *token_lst;
	// t_cmd_lst	*cmd_lst;

	char	*line;

	token_lst = NULL;
	while (1)
	{
		line = readline("minishell: ");
		token_lst = ft_tokenize(line);
		// next step -->
		// token_lst = token_lst_exansion(token_lst);
		ft_print_tokens(token_lst);
		// expansion
		
		// cmd_lst = ms_create_cmd_lst(token_lst);
		// ms_token_free_list(token_lst);
		// execute_cmd(cmd_lst);
		// add_history(line);
		free(line);
	}
	return (0);
}

void	ft_print_tokens(t_token_lst *token_lst)
{
	t_token_lst	*temp;
	int			token_count;

	token_count = 0;
	temp = token_lst;
	while (temp)
	{
		char *type_str;

		// Convert enum type to string
		switch (temp->type)
		{
			case TOKEN_WORD:
				type_str = "WORD";
				break;
			case TOKEN_PIPE:
				type_str = "PIPE";
				break;
			case TOKEN_REDIRECTION_IN:
				type_str = "REDIRECT_IN";
				break;
			case TOKEN_REDIRECTION_OUT:
				type_str = "REDIRECT_OUT";
				break;
			case TOKEN_APPEND:
				type_str = "APPEND";
				break;
			case TOKEN_HEREDOC:
				type_str = "HEREDOC";
				break;
			default:
				type_str = "UNKNOWN";
				break;
		}
		printf("Type: %s\nToken %d: %s===========\n",type_str, token_count, temp->value);
		token_count++;
		temp = temp->next;
	}
}

