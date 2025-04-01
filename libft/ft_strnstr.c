/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:28:41 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/02/03 23:05:17 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (ft_strlen(little) == 0)
		return ((char *)big);
	i = 0;
	while (*(big + i) && i < len)
	{
		if (*(big + i) == *(little))
		{
			j = 0;
			while (*(big + i + j) == *(little + j) && (i + j) < len)
			{
				if (*(little + j + 1) == '\0')
					return ((char *) big + i);
				j++;
			}
		}
		i++;
	}
	return (NULL);
}

// #include <bsd/string.h> // cc -l bsd file.c > to connect files
// #include <stdio.h>
// #define RED "\033[0;31m"  // WRONG
// #define GREEN "\033[0;32m" // CORRECT
// #define PURPLE "\033[0;35m" // ORIGINAL FUNCTION
// #define BLUE "\033[0;34m" // OWN FUNCTION
// #define RESET "\033[0m"
// void	ft_print_strnstr_test(const char *big, const char *little, size_t len)
// {
// 	printf("find: " PURPLE "%s" RESET " in " PURPLE "%s"
// 			RESET " within " PURPLE "%zu" RESET " chars\n", little, big, len);

// 	char	*orig_ptr = strnstr(big, little, len);
// 	char	*ft_ptr =  ft_strnstr(big, little, len);
// 	orig_ptr == ft_ptr
// 	? printf(GREEN"Orig: %p str: %s\nFt  : %p str: %s\n"RESET,
		//orig_ptr, orig_ptr, ft_ptr, ft_ptr)
// 	: printf(RED"Orig: %p str: %s\nFt  : %p str: %s\n"RESET,
//		 orig_ptr, orig_ptr, ft_ptr, ft_ptr);
// }

// int main()
// {
// 	ft_print_strnstr_test("Foo Bar Baz", "Bar", 9);
// 	ft_print_strnstr_test("Foo Bar Baz", "Bar", 4);
// 	ft_print_strnstr_test("Foo Bar Baz", "", 4);
// 	ft_print_strnstr_test("Foo Bar Baz", "bar", 4);
// 	ft_print_strnstr_test("Foo Bar Baz", "Barb", 4);
// 	ft_print_strnstr_test("Foo Bar Baz", " ", 4);
// 	ft_print_strnstr_test("Foo Bar Baz", "\0", 4);
// 	ft_print_strnstr_test("Foo\nBar Baz", "\n", 4);
// 	ft_print_strnstr_test("lorem ipsum dolor sit amet", "dolor", 25);
// 	ft_print_strnstr_test("lorem ipsum dolor sit amet", "dolor", 15);
// 	return (0);
// }
