/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_double_ptr.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 23:33:36 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/02/19 15:15:37 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_free_double_ptr(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (*(str + i))
	{
		free(*(str + i));
		i++;
	}
	free (str);
	return (1);
}
