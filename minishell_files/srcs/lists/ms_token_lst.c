/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_token_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 21:12:14 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/01 12:17:36 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token_lst	*ms_token_new_node(char *value)
{
	t_token_lst	*new_node;

	new_node = (t_token_lst *)malloc(sizeof(t_token_lst));
	if (!new_node)
		return (NULL);
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

void	ms_token_free_list(t_token_lst *head)
{
	t_token_lst	*temp;

	while (head)
	{
		temp = head->next;
		free(head->value);
		free(head);
		head = temp;
	}
}

t_token_lst	*ms_token_get_last_node(t_token_lst *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

void	ms_token_add_node_back(t_token_lst **head, t_token_lst *new_node)
{
	t_token_lst	*last;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	last = ms_token_get_last_node(*head);
	last->next = new_node;
}
