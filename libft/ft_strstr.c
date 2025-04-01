/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 22:57:12 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/02/13 16:39:32 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *big, const char *little)
{
	size_t	i;
	size_t	j;

	if (!ft_strlen(little))
		return ((char *)big);
	i = 0;
	while (*(big + i))
	{
		if (*(big + i) == *little)
		{
			j = 0;
			while (*(big + i + j) == *(little + j))
			{
				j++;
				if (*(little + j) == '\0')
					return ((char *) big + i);
			}
		}
		i++;
	}
	return (NULL);
}
