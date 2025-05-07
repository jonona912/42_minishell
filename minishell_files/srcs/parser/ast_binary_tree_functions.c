/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_binary_tree_functions.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:54:31 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 19:17:21 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/parser.h"

t_ast_node	*create_cmd_node(t_node_type type, char *executable,
	char **exec_argv, t_redir_lst *redirs)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->u_data.s_cmd.executable = executable;
	node->u_data.s_cmd.exec_argv = exec_argv;
	node->u_data.s_cmd.redirs = redirs;
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
	node->u_data.s_binary_op.left = left;
	node->u_data.s_binary_op.right = right;
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
	node->u_data.s_sub_shell.subshell = subshell;
	node->u_data.s_sub_shell.sub_shell_redir = sub_shell_redir;
	return (node);
}

void	free_cmd_node_data(t_ast_node *node)
{
	char	**arg;

	if (!node)
		return ;
	free(node->u_data.s_cmd.executable);
	if (node->u_data.s_cmd.exec_argv)
	{
		arg = node->u_data.s_cmd.exec_argv;
		while (*arg)
		{
			free(*arg);
			arg++;
		}
		free(node->u_data.s_cmd.exec_argv);
	}
	free_redir_list(&node->u_data.s_cmd.redirs);
}

void	free_ast_node(t_ast_node **node)
{
	if (!node || !*node)
		return ;
	if ((*node)->type == NODE_CMD)
	{
		free_cmd_node_data(*node);
	}
	else if ((*node)->type == NODE_PIPE
		|| (*node)->type == NODE_AND || (*node)->type == NODE_OR)
	{
		free_ast_node(&(*node)->u_data.s_binary_op.left);
		free_ast_node(&(*node)->u_data.s_binary_op.right);
	}
	else if ((*node)->type == NODE_SUBSHELL)
	{
		free_ast_node(&(*node)->u_data.s_sub_shell.subshell);
		free_redir_list(&(*node)->u_data.s_sub_shell.sub_shell_redir);
	}
	free(*node);
	*node = NULL;
}
