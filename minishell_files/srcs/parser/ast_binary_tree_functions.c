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

void	free_cmd_node_data(t_ast_node *node)
{
	char	**arg;

	if (!node)
		return ;
	free(node->data.cmd.executable);
	if (node->data.cmd.exec_argv)
	{
		arg = node->data.cmd.exec_argv;
		while (*arg)
		{
			free(*arg);
			arg++;
		}
		free(node->data.cmd.exec_argv);
	}
	free_redir_list(&node->data.cmd.redirs);
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
		free_ast_node(&(*node)->data.binary_op.left);
		free_ast_node(&(*node)->data.binary_op.right);
	}
	else if ((*node)->type == NODE_SUBSHELL)
	{
		free_ast_node(&(*node)->data.sub_shell.subshell);
		free_redir_list(&(*node)->data.sub_shell.sub_shell_redir);
	}
	free(*node);
	*node = NULL;
}
