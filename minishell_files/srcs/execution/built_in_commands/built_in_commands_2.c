/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_commands_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:17:03 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 18:17:04 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/built_in_commands.h"

int	ft_exit(char **argv)
{
	int	i;

	i = 0;
	if (argv[1])
	{
		while (argv[1][i] == '+' || argv[1][i] == '-')
			i++;
		while (argv[1][i])
		{
			if (!ft_isdigit(argv[1][i]))
			{
				ft_putstr_fd("exit: %s: numeric argument required\n", 2);
				exit(2);
			}
			i++;
		}
		if (argv[2])
			return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
		exit(((unsigned int) ft_atoi(argv[1])) % 256);
	}
	exit(0);
}

void	ft_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		ft_putstr_fd("Error: path is not found\n", 2);
}
