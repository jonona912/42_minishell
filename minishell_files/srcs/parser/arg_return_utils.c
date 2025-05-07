/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_return_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:56:24 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 14:56:25 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/parser.h"

char	*dollar_execute(char *name, t_shell shell)
{
	char	*res;

	if (!name)
		return (NULL);
	res = ft_getenv(name, shell);
	if (!res)
		return (NULL);
	return (ft_strdup(res));
}

char	*name_finder(char *value, int pos)
{
	int		i;
	char	*name;

	i = 0;
	pos++;
	while (value[pos + i] && (ft_isalnum(value[pos + i])
			|| value[pos + i] == '_'))
		i++;
	if (i == 0)
		return (NULL);
	name = (char *)malloc(i + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, value + pos, i + 1);
	return (name);
}
