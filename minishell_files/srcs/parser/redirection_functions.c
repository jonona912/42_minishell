/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 23:02:36 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/12 23:06:56 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


t_redir_lst *new_redir_node(t_token_type type, char *target)
{
	t_redir_lst *new_node;

	new_node = (t_redir_lst *)malloc(sizeof(t_redir_lst));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->target = target;
	new_node->next = NULL;
	return (new_node);
}

void add_redir_back(t_redir_lst **lst, t_redir_lst *new_node)
{
	t_redir_lst *temp;

	if (!lst || !new_node)
		return;
	if (!*lst)
	{
		*lst = new_node;
		return;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
}

void free_redir_list(t_redir_lst **lst)
{
	t_redir_lst *temp;
	t_redir_lst *next_node;

	if (!lst || !*lst)
		return;
	temp = *lst;
	while (temp)
	{
		next_node = temp->next;
		if (temp->target)
			free(temp->target);
		free(temp);
		temp = next_node;
	}
	*lst = NULL;
}
