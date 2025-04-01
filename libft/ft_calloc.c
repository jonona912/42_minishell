/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 20:17:34 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/23 20:10:08 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ret;

	if (nmemb && ((nmemb * size) / nmemb) != size)
		return (NULL);
	ret = (void *)malloc(nmemb * size);
	if (!ret)
		return (NULL);
	ft_bzero(ret, nmemb * size);
	return (ret);
}

// int main(void)
// {
// 	printf("%zu\n", sizeof(size_t));
// 	printf("The maximum value of size_t is: %zu\n", SIZE_MAX);
// }
// #include <stdio.h>
// #include <limits.h>
// #include <stdint.h>

// int main() {
//     printf("The maximum value of size_t is: %zu\n", SIZE_MAX);
//     return 0;
// }
