/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:55:21 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/05/03 14:51:10 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isupper(char c)
{
	return (c >= 'A' && c <= 'Z');
}

int	ft_tolower(int c)
{
	int	char_dif;

	char_dif = 'a' - 'A';
	if (ft_isupper(c))
		return (c + char_dif);
	return (c);
}
