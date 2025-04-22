/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr_first.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:51:06 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/17 10:55:23 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr_first(const char *big, const char *little)
{
	size_t	i;
	size_t	little_len;

	little_len = ft_strlen(little);
	if (!little_len)
		return ((char *)big);
	if (ft_strlen(big) < little_len)
		return (NULL);
	i = 0;
	while (*(big + i) == *(little + i))
	{
		i++;
		if (*(little + i) == '\0')
			return ((char *)(big));
	}
	return (NULL);
}
