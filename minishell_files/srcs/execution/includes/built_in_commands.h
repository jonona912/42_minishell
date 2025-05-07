/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_commands.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:31:30 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 18:31:43 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int		ft_cd_special_cases(char **argv,
			t_shell *shell, char **tmp, char *oldpwd);
void	ft_echo_print_loop(char **argv, int *i);
int		ft_exit(char **argv);
int		is_n_flag(char *argv);
int		is_valid_name(char *name);
void	ft_export_env(t_shell shell);
void	handle_no_equal(char **argv, int *exit_status, int *i, t_shell *shell);
int		handle_equal(char **argv, int *exit_status, int *i, t_shell *shell);
