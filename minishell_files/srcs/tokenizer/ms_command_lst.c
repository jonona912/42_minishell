// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   ms_command_lst.c                                   :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/03/30 18:31:40 by zkhojazo          #+#    #+#             */
// /*   Updated: 2025/04/16 16:47:37 by zkhojazo         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../includes/minishell.h"

// t_cmd_lst	*ms_new_node(char *exec_path, char **exec_av)
// {
// 	t_cmd_lst	*node;

// 	node = (t_cmd_lst *)malloc(sizeof(t_cmd_lst));
// 	if (!node)
// 		return (NULL);
// 	node->path = exec_path;
// 	node->av = exec_av;
// 	node->next = NULL;
// 	return (node);
// }

// void	ms_free_list(t_cmd_lst *head)
// {
// 	t_cmd_lst	*temp;

// 	while (head)
// 	{
// 		temp = head->next;
// 		free(head->path);
// 		if (head->av)
// 			ft_free_double_ptr(head->av);
// 		free(head);
// 		head = temp;
// 	}
// }

// t_cmd_lst	*ms_get_last_node(t_cmd_lst *head)
// {
// 	if (!head)
// 		return (NULL);
// 	while (head->next)
// 		head = head->next;
// 	return (head);
// }

// void	ms_add_node_back(t_cmd_lst **head, t_cmd_lst *new_node)
// {
// 	t_cmd_lst	*last;

// 	if (!head || !new_node)
// 		return;
// 	if (!*head)
// 	{
// 		*head = new_node;
// 		return ;
// 	}
// 	last = ms_get_last_node(*head);
// 	last->next = new_node;
// }
