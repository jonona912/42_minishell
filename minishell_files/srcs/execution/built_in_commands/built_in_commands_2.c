#include "../includes/built_in_commands.h"

int	ft_exit(char **argv)
{
	int	i;

	i = 0;
	if (argv[1])
	{
		while (argv[1][i])
		{
			if (!ft_isdigit(argv[1][i]))
			{
				printf("exit: %s: numeric argument required\n", argv[1]);
				exit(2);
			}
			i++;
		}
		if (argv[2])
			return (printf("exit: too many arguments\n"), 1);
		exit(ft_atoi(argv[1]) % 256);
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
