/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 19:15:42 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/03/31 21:12:01 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	acum;
	int	sign;

	sign = 1;
	acum = 0;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		acum = acum * 10 + (*nptr - '0');
		nptr++;
	}
	return (acum * sign);
}

// #include <stdlib.h>
// #include <stdio.h>

// int main()
// {
// 	char *str1 = "1234";
// 	char *str2 = "-1234";
// 	char *str3 = "-12a132";
// 	char *str4 = "--1234";
// 	char *str5 = "-2147483648";
// 	char *str6 = "2147483648";
// 	char *str7 = "2147483647";
// 	char *str8 = "+12adf";
// 	char *str9 = "++12adf";
// 	char *str10 = "\t 	+12adf";
// 	char *str11 = "\010 9";
// 	char *str_array[] = {str1, str2, str3, str4, 
// 	str5, str6, str7, str8, str9, str10, str11};
// 	int i = 0;
// 	while (i < 11)
// 	{
// 		printf("%i: Orig: %i ft: %i\n", i + 1, 
// 	atoi(str_array[i]), ft_atoi(str_array[i]));
// 		i++;
// 	}
// 	// ft_atoi("\010 9");
// 	return (0);
// }