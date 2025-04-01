/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 20:46:29 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/21 19:32:19 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n <= 0)
		return (0);
	i = 0;
	while (i < n && *(s1 + i) && (*(s1 + i) == *(s2 + i)))
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)*(s1 + i) - (unsigned char)*(s2 + i));
}

// #define RED "\033[0;31m"  // WRONG
// #define GREEN "\033[0;32m" // CORRECT
// #define PURPLE "\033[0;35m" // ORIGINAL FUNCTION
// #define BLUE "\033[0;34m" // OWN FUNCTION
// #define RESET "\033[0m"
// void ft_print_strncmp_test(const char *s1, const char *s2, size_t n)
// {
// 	int	orig;
// 	int	ft;
// 	orig = strncmp(s1, s2, n);
// 	ft = ft_strncmp(s1, s2, n);
// 	orig == ft
// 	? printf(GREEN"Orig: %i\nFt  : %i\n"RESET, orig, ft)
// 	: printf(RED"Orig: %i\nFt  : %i\n"RESET, orig, ft);
// }

// int main()
// {
// 	ft_print_strncmp_test("zyxbcdefgh", "abcdwxyz", 4);
// 	ft_print_strncmp_test("abcdefgh", "", 0);	
// 	ft_print_strncmp_test("abcdefgh", "abcdwxyz", 4);
// 	ft_print_strncmp_test("test\200", "test\0", 6);
// 	return (0);
// }
