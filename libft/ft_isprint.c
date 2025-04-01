/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:04:25 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/11 12:19:47 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isprint(int c)
{
	return (c >= 32 && c <= 126);
}

// #include <ctype.h>
// #include <stdio.h>
// int main()
// {
// 	int	c = 0;
// 	printf("Orig: %i ft: %i\n", isprint(c), ft_isprint(c));
// 	int b = 'z';
// 	printf("Orig: %i ft: %i\n", isprint(b), ft_isprint(b));
// 	b = 126;
// 	printf("Orig: %i ft: %i\n", isprint(b), ft_isprint(b));
// 	b = 127;
// 	printf("Orig: %i ft: %i\n", isprint(b), ft_isprint(b));
// 	b = -1;
// 	printf("Orig: %i ft: %i\n", isprint(b), ft_isprint(b));

// 	return (0);
// }