/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:58:40 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/05/03 14:51:20 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_islower(int c)
{
	return (c >= 'a' && c <= 'z');
}

static int	ft_isupper(int c)
{
	return (c >= 'A' && c <= 'Z');
}

int	ft_isalpha(int c)
{
	return (ft_isupper(c) || ft_islower(c));
}

// int	ft_isalpha(char c)
// {
// 	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
// }

// #include <stdio.h>
// #include <ctype.h>

// int	main()
// {
// 	char	c = 'a';
// 	printf("Orig: %i ft: %i\n", isalpha(c), ft_isalpha(c));
// 	c = '2';
// 	printf("Orig: %i ft: %i\n", isalpha(c), ft_isalpha(c));
// 	return (0);
// }
