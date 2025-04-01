/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 20:20:12 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/23 21:01:45 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ret;

	ret = NULL;
	while (*s)
	{
		if (*s == (char)c)
			ret = (char *)s;
		s++;
	}
	if (*s == (char)c)
		return ((char *) s);
	return (ret);
}

// #include <stdio.h>
// int main()
// {
// 	char	s_chr[] = "ab#~da1#@jdE#D$#/#asdfasdf";
// 	char	c_find = '#';
// 	printf("%s\n", strrchr(s_chr, c_find));
// }