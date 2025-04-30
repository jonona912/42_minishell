/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 11:07:14 by opopov            #+#    #+#             */
/*   Updated: 2025/04/29 19:48:27 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token_lst	*token_new_node(t_token_type type, char *value)
{
	t_token_lst	*new_node;

	new_node = (t_token_lst *)malloc(sizeof(t_token_lst));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->value = value;
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

int	token_lst_size(t_token_lst *head)
{
	int	count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return (count);
}
