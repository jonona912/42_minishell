/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:48:00 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/17 17:37:33 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*ret;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	ret = (char *)ft_calloc((s1_len + s2_len + 1), sizeof(char));
	if (ret == NULL)
		return (ret);
	ft_strcpy(ret, (char *) s1);
	ft_strcpy((ret + s1_len), (char *) s2);
	return (ret);
}
