/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:06:32 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/23 20:25:42 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*ret;
	unsigned int	i;
	size_t			s_len;

	i = 0;
	s_len = ft_strlen(s);
	if (s_len < start)
		return (ft_calloc(1, 1));
	s_len = ft_strlen(s + start);
	if (s_len < len)
		len = s_len;
	ret = malloc((len + 1) * sizeof(char));
	if (ret == NULL)
		return (ret);
	while (i < len && *(s + start))
	{
		*(ret + i) = *(s + start);
		i++;
		start++;
	}
	*(ret + i) = '\0';
	return (ret);
}
