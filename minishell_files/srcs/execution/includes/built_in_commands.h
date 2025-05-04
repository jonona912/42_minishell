#include "execution.h"

int		ft_cd_special_cases(char **argv, t_shell *shell, char **tmp, char *oldpwd);
void	ft_echo_print_loop(char **argv, int *i);
int		ft_exit(char **argv);
int		is_n_flag(char *argv);
int		is_valid_name(char *name);
// int		ft_cd_end(char *curr_pwd, t_shell *shell, char **oldpwd, char *cwd);
void	ft_export_env(t_shell shell);
