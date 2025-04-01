/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 11:20:07 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/24 18:25:27 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	s_len;
	unsigned int	s_id;
	char			*ret;

	s_len = ft_strlen(s);
	s_id = 0;
	ret = (char *)ft_calloc((s_len + 1), sizeof(char));
	if (!ret)
		return (ret);
	while (*(s + s_id))
	{
		*(ret + s_id) = f(s_id, *(s + s_id));
		s_id++;
	}
	*(ret + s_id) = '\0';
	return (ret);
}
