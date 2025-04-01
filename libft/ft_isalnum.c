/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:44:52 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/22 20:54:24 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

// #include <stdio.h>
// #include <ctype.h>

// int main()
// {
// 	char c = 'a';
// 	printf("Orig: %i ft: %i\n", isalnum(c), ft_isalnum(c));
// 	c = '9';
// 	printf("Orig: %i ft: %i\n", isalnum(c), ft_isalnum(c));
// 	c = '!';
// 	printf("Orig: %i ft: %i\n", isalnum(c), ft_isalnum(c));
// 	c = ' ';
// 	printf("Orig: %i ft: %i\n", isalnum(c), ft_isalnum(c));
// 	char *s = "012345";
// 	printf("strlen Demo for Guru Jee: %i\n", ft_strlen(s));
// }
