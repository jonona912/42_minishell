/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:47:18 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/17 15:39:22 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_islower(char c)
{
	return (c >= 'a' && c <= 'z');
}

int	ft_toupper(int c)
{
	int	char_dif;

	char_dif = 'a' - 'A';
	if (ft_islower(c))
		return (c - char_dif);
	return (c);
}
