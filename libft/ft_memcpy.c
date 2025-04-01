/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:57:57 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/26 14:33:42 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s;

	if (!dest && !src)
		return (NULL);
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

// #include <stdio.h>
// #include <string.h>

// int main()
// {
//     // char str1[] = "Geeks";
//     // char str2[] = "Quiz";
//     // puts("str1 before memcpy ");
//     // puts(str1);

//     // // Copies contents of str2 to str1
//     // memcpy(str1, str2, sizeof(str2));

//     // puts("\nstr1 after memcpy ");
//     // puts(str1);
// 	char dest_str[] = "Tutorialspoint"; 
// 	char src_str[] = "Tutors";
// 	printf("%s\n", dest_str);
// 	memcpy(dest_str, dest_str + 1, sizeof(src_str));
// 	printf("Orig: %s\n", dest_str); 
// 	char dest_str2[] = "Tutorialspoint";
// 	ft_memcpy(dest_str2, dest_str2 + 1, sizeof(src_str));
// 	printf("Ft  : %s\n", dest_str2);
// 	ft_memcpy(((void*)0), ((void*)0), 3);
// 	memcpy(((void*)0), ((void*)0), 3);

// 	return 0;
// }

// void	*ft_memcpy(void *dest, const void *src, size_t n)
// {
// 	if (!dest || !src)
// 		return (NULL);
// 	while (n--)
// 		*((unsigned char *)(dest + n)) = *((unsigned char *)(src + n));
// 	return (dest);
// }

// void	*ft_memcpy(void *dest, const void *src, size_t n)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!dest || !src)
// 		return (NULL);
// 	while (i < n && (src + i) && (dest + i))
// 	{
// 		// *((unsigned char *)(dest) + i) = *((unsigned char *)(src) + i);
// 		 *((unsigned char *)dest + i) = *((unsigned char *)src + i);
// 		i++;
// 	}
// 	return (dest);
// }
