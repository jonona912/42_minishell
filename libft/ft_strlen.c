/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 20:42:44 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/17 17:37:42 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	ctr;

	ctr = 0;
	if (!str)
		return (0);
	while (*str)
	{
		ctr++;
		str++;
	}
	return (ctr);
}

// #include <stdio.h>
// #include <string.h>
// int main(void)
// {
// 	char str1[] = "This is my second day of\t11 !@#(8*7&)\t";
// 	char str2[] = "";
// 	char str3 = 'a';
// 	printf("Orig: %lu str1: %i\n", strlen(str1), ft_strlen(str1));
// 	printf("Orig: %lu str2: %i\n", strlen(str2), ft_strlen(str2));
// 	printf("Orig: %lu str3: %i\n", strlen(&str3), ft_strlen(&str3));
// 	return (0);
// }
