/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_binary_tree_functions.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:42:58 by opopov            #+#    #+#             */
/*   Updated: 2025/04/26 15:43:13 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*create_cmd_node(t_node_type type, char *executable,
	char **exec_argv, t_redir_lst *redirs)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->data.cmd.executable = executable;
	node->data.cmd.exec_argv = exec_argv;
	node->data.cmd.redirs = redirs;
	return (node);
}

t_ast_node	*create_binary_op_node(t_node_type type,
	t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->data.binary_op.left = left;
	node->data.binary_op.right = right;
	return (node);
}

t_ast_node	*create_subshell_node(t_node_type type,
	t_ast_node *subshell, t_redir_lst *sub_shell_redir)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->data.sub_shell.subshell = subshell;
	node->data.sub_shell.sub_shell_redir = sub_shell_redir;
	return (node);
}
