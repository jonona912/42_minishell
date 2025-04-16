/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 13:10:10 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/15 12:47:33 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// static char	*ft_strcpy(char *dest, const char *src)
// {
// 	char	*ret;

// 	ret = dest;
// 	while (*src)
// 	{
// 		*dest = *src;
// 		src++;
// 		dest++;
// 	}
// 	*dest = '\0';
// 	return (ret);
// }

static int	ft_numlen(int n)
{
	int	ret;

	ret = 0;
	if (n <= 0)
		ret = 1;
	while (n)
	{
		n /= 10;
		ret++;
	}
	return (ret);
}

static void	ft_revstring(char *s)
{
	int	s_len;
	int	iter;
	int	start;

	start = 0;
	s_len = ft_strlen(s);
	iter = s_len / 2;
	while (start < iter)
	{
		*(s + start) = *(s + start) ^ *(s + s_len - 1);
		*(s + s_len - 1) = *(s + start) ^ *(s + s_len - 1);
		*(s + start) = *(s + start) ^ *(s + s_len - 1);
		start++;
		s_len--;
	}
}

static void	ft_strappend(char *ret, int n, int i)
{
	int		sign;

	sign = 1;
	if (n < 0)
	{
		sign *= (-1);
		n *= sign;
	}
	while (n > 0)
	{
		*(ret + i) = n % 10 + '0';
		n = n / 10;
		i++;
	}
	if (sign < 0)
		*(ret + i++) = '-';
	*(ret + i) = '\0';
	ft_revstring(ret);
}

char	*ft_itoa(int n)
{
	char	*ret;
	int		i;

	i = 0;
	ret = (char *)ft_calloc((ft_numlen(n) + 1), sizeof(char));
	if (!ret)
		return (NULL);
	if (n == -2147483648)
		ft_strcpy(ret, "-2147483648");
	else if (n == 0)
	{
		*ret = '0';
		i++;
	}
	else
		ft_strappend(ret, n, i);
	return (ret);
}
