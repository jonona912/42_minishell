/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:50:26 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 19:19:08 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/parser.h"

t_token_lst	*handle_cmd_redir(t_token_lst *token_lst,
								t_redir_lst **redir_lst, t_shell *shell)
{
	t_redir_lst		*redir_temp;
	t_token_type	type;
	char			*temp_str;

	(void)shell;
	type = token_lst->type;
	token_lst = token_lst->next;
	if (token_lst && (token_lst->type == TOKEN_WORD
			|| token_lst->type == TOKEN_S_QUOTE
			|| token_lst->type == TOKEN_D_QUOTE))
	{
		temp_str = ft_strdup(token_lst->value);
		if (!temp_str)
			return (NULL);
		redir_temp = new_redir_node(type, temp_str);
		if (!redir_temp)
			return (free(temp_str), NULL);
		add_redir_back(redir_lst, redir_temp);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error2\n", 2);
		return (NULL);
	}
	return (token_lst);
}

t_token_lst	*make_cmd_and_redir_lst(t_token_lst *token_lst,
									t_token_lst **cmd_lst,
									t_redir_lst **redir_lst, t_shell *shell)
{
	t_token_lst		*temp;
	char			*temp_str;

	while (is_cmd_valid(token_lst->type))
	{
		if (is_token_valid_for_cmd(token_lst->type))
		{
			temp_str = ft_strdup(token_lst->value);
			temp = token_new_node(token_lst->type, temp_str);
			if (!temp)
				return (free(temp_str), NULL);
			token_add_node_back(cmd_lst, temp);
		}
		else if (is_redirection(token_lst->type))
		{
			token_lst = handle_cmd_redir(token_lst, redir_lst, shell);
			if (!token_lst)
				return (NULL);
		}
		else
			return (ft_putstr_fd("minishell: syntax error\n", 2), NULL);
		token_lst = token_lst->next;
	}
	return (token_lst);
}

int	allocate_exec_argv(t_ast_node **ast_node, int size)
{
	(*ast_node)->u_data.s_cmd.exec_argv
		= (char **) malloc((size + 1) * sizeof(char *));
	if (!(*ast_node)->u_data.s_cmd.exec_argv)
		return (-1);
	return (0);
}

int	set_executable_and_argv(t_ast_node **ast_node,
							t_token_lst *cmd_lst,
							t_shell *shell)
{
	int	ctr;

	(void)shell;
	if (!cmd_lst)
		return (0);
	ctr = token_lst_size(cmd_lst);
	if (ctr == 0)
		return (0);
	if (allocate_exec_argv(ast_node, ctr) == -1)
		return (-1);
	if (builtin_check(cmd_lst->value))
		(*ast_node)->u_data.s_cmd.executable
			= ft_strdup(cmd_lst->value);
	else
	{
		(*ast_node)->u_data.s_cmd.executable = return_executable_path
			(cmd_lst->value, shell);
		if (!(*ast_node)->u_data.s_cmd.executable)
			return (-1);
	}
	ctr = cmd_lst_loop(cmd_lst, ast_node);
	return ((*ast_node)->u_data.s_cmd.exec_argv[ctr] = NULL, 0);
}

t_token_lst	*parse_cmd(t_token_lst *token_lst,
						t_ast_node **ast_node,
						t_shell *shell)
{
	t_token_lst	*cmd_lst;

	cmd_lst = NULL;
	*ast_node = create_cmd_node(NODE_CMD, NULL, NULL, NULL);
	token_lst = make_cmd_and_redir_lst(token_lst,
			&cmd_lst, &(*ast_node)->u_data.s_cmd.redirs, shell);
	if (!token_lst)
	{
		token_free_list(cmd_lst);
		return (NULL);
	}
	if (set_executable_and_argv(ast_node, cmd_lst, shell) == -1)
	{
		token_free_list(cmd_lst);
		return (NULL);
	}
	token_free_list(cmd_lst);
	return (token_lst);
}
