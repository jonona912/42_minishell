/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:38:58 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/14 10:57:18 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	while (n--)
		((unsigned char *)s)[n] = (unsigned char) c;
	return (s);
}

// #include <string.h>
// #include <stdio.h>

// void printArray(int arr[], int n)
// {
//    for (int i=0; i<n; i++)
//       printf("%d ", arr[i]);
// }

// int main()
// {
// 	char s_orig[] = "012345678911";
// 	char s_ft[] = "012345678911";
// 	int i = 126;
// 	size_t num = 5 * sizeof(char);
// 	memset(s_orig + 3, i, num);
// 	ft_memset(s_ft + 3, i, num);
// 	printf("Orig: %s\n", s_orig);
// 	printf("Ft  : %s\n", s_ft);

//     int n = 10;
//     int arr = 1;

//     // Fill whole array with 100.
//     memset(&arr, 10, sizeof(arr));
//     printf("Array after memset() %i\n", arr);
//     // printArray(arr, n);
//     int n_ft = 10;
//     int arr_ft[n];

//     // Fill whole array with 100.
//     ft_memset(arr_ft, 10, n_ft*sizeof(arr_ft[0]));
//     printf("\nArray after memset()\n");
//     // printArray(arr_ft, n_ft);

// 	return (0);
// }

// // #####################################
// // Code written by myself for reference
// void	*ft_memset(void *s, int c, size_t n)
// {
// 	size_t			i;
// 	unsigned char	*c_typecast;

// 	i = 0;
// 	c_typecast = (unsigned char *) s;
// 	while (i < n)
// 	{
// 		*(c_typecast + i) = (unsigned char) c;
// 		i++;
// 	}
// 	return ((void *)c_typecast);
// }
