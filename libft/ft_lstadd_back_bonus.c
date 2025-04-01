/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 21:25:01 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/26 12:14:05 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (new == NULL)
		return ;
	last = ft_lstlast(*lst);
	if (last == NULL)
		*lst = new;
	else
		last->next = new;
}

// // Add a node to the end of the list
// void	ft_lstadd_back(t_list **lst, t_list *new)
// {
//     t_list	*temp;

//     if (!lst || !new)
//         return;
//     if (!*lst)
//     {
//         *lst = new;
//         return;
//     }
//     temp = *lst;
//     while (temp->next)
//         temp = temp->next;
//     temp->next = new;
// }

// ---------------------------------------------------------
// // debuging with Polina -> was really nice to debug
// #include <stdio.h>
// int main(void)
// {

// 	t_list * l =  NULL; t_list * l2 =  NULL;
// 	int one = 1;
// 	int two = 2;
// 	int three = 3;
// 	int four = 4;
// 	ft_lstadd_back(&l, ft_lstnew((void*)&one));
// 	ft_lstadd_back(&l, ft_lstnew((void*)&two));

// 	ft_lstadd_back(&l2, ft_lstnew((void*)&three));
// 	ft_lstadd_back(&l2, ft_lstnew((void*)&four));
// 	printf("two: %d\n", *(int *)l2->next->content);
// 	ft_lstadd_back(&l, l2);
// 	printf("res: %d\n",
//	*(int *)((t_list *)((t_list *)(((t_list *)
//	(l->next))->next)->next))->content);

// 	write(1, "\n", 1);
// 	return (0);
// }

// void	ft_lstadd_back(t_list **lst, t_list *new)
// {
// 	t_list	*last;

// 	if (new == NULL)
// 		return;
// 	last = ft_lstlast(*lst);
// 	if (last == NULL)
// 	{
// 		*lst = new;
// 		new->next = NULL;
// 	}
// 	else
// 	{
// 		last->next = new;
// 		// new->next = NULL;
// 	}
// }
// --------------------------------------------------------------