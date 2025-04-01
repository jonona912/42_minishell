/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:25:58 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/24 12:33:05 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*s_cast;

	s_cast = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if ((*(s_cast + i)) == (unsigned char)c)
			return ((void *)(s_cast + i));
		i++;
	}
	return (NULL);
}
