/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_three.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:44:12 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/15 12:45:43 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	s3_len;
	char	*ret;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	s3_len = ft_strlen(s3);
	ret = (char *)ft_calloc((s1_len + s2_len + s3_len + 1), sizeof(char));
	if (ret == NULL)
		return (ret);
	ft_strcpy(ret, (char *) s1);
	ft_strcpy((ret + s1_len), (char *) s2);
	ft_strcpy((ret + s1_len + s2_len), (char *) s3);
	return (ret);
}
