/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:53:41 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/16 13:09:24 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long long	i;

	i = n;
	if (i < 0)
	{
		ft_putchar_fd('-', fd);
		i *= (-1);
	}
	if (i > 9)
		ft_putnbr_fd(i / 10, fd);
	ft_putchar_fd((char)((i % 10) + '0'), fd);
}
