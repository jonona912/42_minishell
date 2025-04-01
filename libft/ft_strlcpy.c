/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 22:51:11 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/26 14:36:07 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	src_len = ft_strlen(src);
	if (size == 0)
		return (src_len);
	while (i < (size - 1) && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}

// #include "bsd/string.h"
// #define RED "\033[0;31m"  // WRONG
// #define GREEN "\033[0;32m" // CORRECT
// #define PURPLE "\033[0;35m" // ORIGINAL FUNCTION
// #define BLUE "\033[0;34m" // OWN FUNCTION
// #define RESET "\033[0m"
// void ft_print_strlcpy_test(char	*dst_orig1, 
// char *dst_ft1, char *str_cpy, size_t len)
// {
// 	printf(BLUE"\nft_strlcpy: %s Len: %zu\n"RESET, str_cpy, len);
// 	strlcpy(dst_orig1, str_cpy, len);
// 	ft_strlcpy(dst_ft1, str_cpy, len);
// 	(strcmp(dst_orig1, dst_ft1) == 0)
// 	? printf(GREEN"Orig: %s\nFt  : %s\n"RESET, dst_orig1, dst_ft1)
// 	: printf(RED"Orig: %s\nFt  : %s\n"RESET, dst_orig1, dst_ft1);
// }

// int main()
// {
// 	char	dst_orig1[20];
// 	char	dst_ft1[20];
// 	char	str_cpy[] = "~2r4^67*911V21517af21";
// 	size_t 	len = 12;
// 	ft_print_strlcpy_test(dst_orig1, dst_ft1, str_cpy, len);
// 	ft_print_strlcpy_test(dst_orig1, dst_ft1, "something", len);
// 	ft_print_strlcpy_test(dst_orig1, dst_ft1, "", len);
// 	ft_print_strlcpy_test(dst_orig1, dst_ft1, "a", len);
// 	ft_print_strlcpy_test(dst_orig1, dst_ft1, "!\t \n$#*#^%", len);
// 	char	dst_orig2[20];
// 	char	dst_ft2[20];
// 	printf("dst_int %i\n", *dst_orig2);
// 	ft_print_strlcpy_test(dst_orig2, dst_ft2, "lorem ipsum dolor sit amet", 0);
// 	printf("dst_int %i\n", *dst_orig2);
// }
