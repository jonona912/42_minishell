/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:45:06 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/24 21:54:34 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*nxt;

	if (!lst || !*lst)
		return ;
	nxt = *lst;
	while (*lst != NULL)
	{
		nxt = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = nxt;
	}
}

// // Clear the list and free allocated memory
// void	ft_lstclear(t_list **lst, void (*del)(void *))
// {
//     t_list	*temp;

//     if (!lst || !del)
//         return;
//     while (*lst)
//     {
//         temp = (*lst)->next;
//         del((*lst)->content); // Free content using provided delete function
//         free(*lst);          // Free the node itself
//         *lst = temp;
//     }
//     *lst = NULL; // Ensure the list pointer is NULL after clearing
// }