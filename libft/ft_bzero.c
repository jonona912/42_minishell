/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:37:15 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/13 12:37:31 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

// int	main()
// {
// 	/// At 9 ft doesn't match original
// 	size_t num = 3 * sizeof(char);
// 	int nth_pos = 3;

// 	char s_orig[] = "012345678911";
// 	bzero(s_orig + nth_pos, num);
// 	printf("Orig: %s\n", s_orig);

// 	char s_ft[] = "012345678911";
// 	ft_bzero(s_ft + nth_pos, num);
// 	printf("FT  : %s\n", s_ft);

// 	return (0);
// }
