/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:46:15 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/23 19:41:43 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	unsigned const char	*s1_cast;
	unsigned const char	*s2_cast;

	s1_cast = (unsigned const char *)s1;
	s2_cast = (unsigned const char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while ((*(s1_cast + i) == *(s2_cast + i)) && (i < n - 1))
		i++;
	return (*(s1_cast + i) - *(s2_cast + i));
}
