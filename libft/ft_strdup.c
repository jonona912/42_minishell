/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 22:27:43 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/02/18 14:42:18 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// The strdup() function returns a pointer to a new string 
// which is a duplicate of the string s.  Memory for the 
// new string is obtained with malloc(3), and can be freed with free(3).
#include <stdlib.h>
#include "libft.h" // for ft_strlen

char	*ft_strdup(char *src)
{
	int		str_len;
	char	*str_ptr;
	char	*temp_ptr;

	str_len = ft_strlen(src);
	str_ptr = malloc(str_len + 1);
	if (!str_ptr)
		return (NULL);
	temp_ptr = str_ptr;
	while (*src)
	{
		*str_ptr = *src;
		str_ptr++;
		src++;
	}
	*str_ptr = '\0';
	return (temp_ptr);
}

// #include <stdio.h>
// #include <string.h>

// int main(void)
// {
// 	char str[] = "abcdefghijkl";
// 	char *str_cp;
// 	char *str_cp2;
// 	// printf("STR: %s\n", str_cp);
// 	str_cp = ft_strdup(str);
// 	printf("STR: %s\n", str_cp);
// 	str_cp2 = ft_strdup(str);
// 	printf("Orig: %s\n", str_cp2);
// 	return (0);
// }
