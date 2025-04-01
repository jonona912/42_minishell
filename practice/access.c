#include <unistd.h>
#include <stdio.h>

int main(void)
{
	int no_exe = access("access_test", F_OK | R_OK);
	printf("no_exe %d\n", no_exe);
	printf("yes_ex %d\n", access("a.out", R_OK | W_OK));
	printf("F_OK | R_OK: %d\n", R_OK | W_OK | X_OK);
	printf("F_OK       : %d\n", W_OK);
	printf("R_OK	   : %d\n", R_OK);
	return (0);
}

