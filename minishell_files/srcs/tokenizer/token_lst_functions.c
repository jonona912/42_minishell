#include "../../includes/minishell.h"

t_token_lst	*token_new_node(t_token_type type, char *value)
{
	t_token_lst	*new_node;

	new_node = (t_token_lst *)malloc(sizeof(t_token_lst));
	if (!new_node)
		return (NULL); // restart the branches
	new_node->type = type; // t_token_type -> ls -l | grep "test" ls TOKEN_WORD -l TOKEN_WORD | TOKEN_PIPE grep TOKEN_WORD "test" TOKEN_D_QUOTE (blank spaces between tokens skipped)
	new_node->value = value; // ls       -l      |      grep        test (IMPORTANT TO REMOVE QUOTES)
	new_node->next = NULL;
	return (new_node);
}

void	token_free_list(t_token_lst *head)
{
	t_token_lst	*temp;

	while (head)
	{
		temp = head->next;
		if (head->value)
		{
			free(head->value);
			head->value = NULL;
		}
		free(head);
		head = temp;
	}
}

t_token_lst	*token_get_last_node(t_token_lst *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

void	token_add_node_back(t_token_lst **head, t_token_lst *new_node)
{
	t_token_lst	*last;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	last = token_get_last_node(*head);
	last->next = new_node;
}
