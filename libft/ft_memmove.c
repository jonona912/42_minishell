/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:43:20 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/26 14:33:50 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h> 

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	if (!dest && !src)
		return (NULL);
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (d < s)
	{
		ft_memcpy(dest, src, n);
	}
	else
	{
		d += n;
		s += n;
		while (n--)
			*--d = *--s;
	}
	return (dest);
}

// #include <string.h> 

// int main() 
// { 
// 	char dest_str[] = "HelloWorldWideWeb"; 
// 	char src_str[] = "Bar";
// 	printf("Orig: %s\n", dest_str);
// 	memmove(dest_str, dest_str + 3, 6 * sizeof(char));
// 	printf("Orig: %s\n", dest_str);
// 	char dest_str2[] = "HelloWorldWideWeb";
// 	ft_memmove(dest_str2, dest_str2 + 3, 6 *sizeof(char));
// 	printf("Ft  : %s\n", dest_str2);
// 	return 0; 
// }

// // my first solution
// void	*ft_memmove(void *dest, const void *src, size_t n)
// {
// 	unsigned char src_cpy[n];
// 	size_t	i;

// 	i = 0;
// 	while (i < n)
// 	{
// 		*(src_cpy + i) = *((unsigned char *)(src + i));
// 		i++;
// 	}
// 	while (n--)
// 	{
// 		*((unsigned char *)(dest + n)) = *(src_cpy + n);
// 	}
// 	return (dest);
// }

// void	*ft_memmove(void *dest, const void *src, size_t n)
// {
// 	if (dest < src)
// 		ft_memcpy(dest, src, n);
// 	else
// 	{
// 		while (n--)
// 			*((unsigned char *)(dest) + n) = *((unsigned char *)(src) + n);
// 	}
// 	return (dest);
// }