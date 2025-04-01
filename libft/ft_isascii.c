/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:57:05 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/11 12:16:20 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}

// #include <ctype.h>
// #include <stdio.h>
// int main()
// {
// 	int	c = 0;
// 	printf("Orig: %i ft: %i\n", isascii(c), ft_isascii(c));
// 	int b = 12;
// 	printf("Orig: %i ft: %i\n", isascii(b), ft_isascii(b));
// 	b = 127;
// 	printf("Orig: %i ft: %i\n", isascii(b), ft_isascii(b));
// 	b = 128;
// 	printf("Orig: %i ft: %i\n", isascii(b), ft_isascii(b));
// 	b = -1;
// 	printf("Orig: %i ft: %i\n", isascii(b), ft_isascii(b));

// 	return (0);
// }