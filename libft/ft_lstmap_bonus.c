/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 17:24:31 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/24 18:42:31 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*temp;
	t_list	*newlist;
	void	*cont;

	newlist = NULL;
	if (!lst || !f || !del)
		return (NULL);
	while (lst)
	{
		cont = f(lst->content);
		if (!cont)
			return (ft_lstclear(&newlist, del), NULL);
		temp = ft_lstnew(cont);
		if (!temp)
		{
			free(cont);
			ft_lstclear(&newlist, del);
			return (NULL);
		}
		ft_lstadd_back(&newlist, temp);
		lst = lst->next;
	}
	return (newlist);
}
// =====================================================================

// // // // cc ft_lstmap.c ft_lstnew.c ft_lstclear.c 
// // // // ft_lstadd_back.c ft_lstdelone.c ft_lstlast.c && ./a.out
// #include <stdio.h>
// #include <stdlib.h>

// // // Assume these are defined in your project
// // typedef struct s_list
// // {
// //     void            *content;
// //     struct s_list   *next;
// // }                   t_list;

// t_list  *ft_lstnew(void *content);
// void    ft_lstadd_back(t_list **lst, t_list *new);
// void    ft_lstclear(t_list **lst, void (*del)(void *));
// void    *test_function(void *content);
// void    test_delete_function(void *content);
// t_list  *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

// int allocation_error = 0; // Control variable to simulate allocation error

// void *test_function(void *content)
// {
//     return content; // Just return the content (identity function)
// }

// void test_delete_function(void *content)
// {
//     free(content);
// }

// // t_list *ft_lstnew(void *content)
// // {
// //     t_list *new_node;

// //     if (allocation_error) // Simulate memory allocation failure
// //         return NULL;
// //     new_node = (t_list *)malloc(sizeof(t_list));
// //     if (!new_node)
// //         return NULL;
// //     new_node->content = content;
// //     new_node->next = NULL;
// //     return new_node;
// // }

// // Helper to create a simple list
// t_list *create_test_list(void)
// {
//     t_list *head = ft_lstnew(malloc(sizeof(int)));
//     *(int *)head->content = 1;

//     t_list *second = ft_lstnew(malloc(sizeof(int)));
//     *(int *)second->content = 2;
//     ft_lstadd_back(&head, second);

//     t_list *third = ft_lstnew(malloc(sizeof(int)));
//     *(int *)third->content = 3;
//     ft_lstadd_back(&head, third);

//     return head;
// }

// // Test to trigger ft_lstclear
// void test_ft_lstmap_error_handling(void)
// {
//     t_list *lst = create_test_list();
//     allocation_error = 1; // Simulate allocation error for ft_lstnew
//     t_list *mapped = ft_lstmap(lst, test_function, test_delete_function);

//     if (!mapped)
//         printf("ft_lstmap failed as expected, ft_lstclear was triggered.\n");

//     ft_lstclear(&lst, test_delete_function); // Clean up original list
// }

// int main(void)
// {
//     test_ft_lstmap_error_handling();
//     return 0;
// }

// t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
// {
// 	t_list	*new_list;
// 	t_list	*new_node;

// 	if (!lst || !f)
// 		return (NULL);
// 	new_list = NULL;  // Initialize the new list
// 	while (lst)
// 	{
// 		// Apply the function `f` to the content of the current node
// 		new_node = ft_lstnew(f(lst->content));
// 		if (!new_node)
// 		{
// 			// Clear the new list and free allocated memory if allocation fails
// 			ft_lstclear(&new_list, del);
// 			return (NULL);
// 		}
// 		// Add the new node to the new list
// 		ft_lstadd_back(&new_list, new_node);
// 		lst = lst->next;  // Move to the next node
// 	}
// 	return (new_list);
// }
