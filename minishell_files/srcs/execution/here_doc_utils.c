/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:07:36 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 15:12:54 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/execution.h"

void	handle_heredoc_input(char *input, int in_fd)
{
	ft_putstr_fd(input, in_fd);
	ft_putstr_fd("\n", in_fd);
	free(input);
}

int	handle_heredoc_eof(struct sigaction *old_sa)
{
	sigaction(SIGINT, old_sa, NULL);
	ft_putstr_fd("warning: here-doc delimited by end-of-file\n", 2);
	return (1);
}

int	ms_strcmp_until(char *s1, char *s2, char c)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (*(s1 + i) && *(s2 + i) && *(s1 + i) != c && *(s1 + i) == *(s2 + i))
		i++;
	return (*(s1 + i) - *(s2 + i));
}
