/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr_last.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:17:59 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/02/13 16:39:28 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr_last(const char *big, const char *little)
{
	size_t	i;
	size_t	little_len;
	size_t	big_len;

	little_len = ft_strlen(little);
	if (!little_len)
		return ((char *)big);
	big_len = ft_strlen(big);
	if (big_len < little_len)
		return (NULL);
	big_len -= little_len;
	i = 0;
	while (*(big + big_len + i) == *(little + i))
	{
		i++;
		if (*(little + i) == '\0')
			return ((char *) big + big_len);
	}
	return (NULL);
}
