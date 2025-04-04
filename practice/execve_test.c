


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
	extern char	**environ;
	// int fd = open("employee.txt", O_RDONLY);
	// if (fd < 0) {
	// 	perror("open");
	// 	return 1;
	// }

	// if (dup2(fd, STDIN_FILENO) < 0) {
	// 	perror("dup2");
	// 	close(fd);
	// 	return 1;
	// }

	// close(fd);

	// char *args[] = {"/bin/echo", "$PATH", NULL};
	// if (execve("/bin/echo", args, NULL) < 0) {
	// 	perror("execve");
	// 	return 1;
	// }
	char *args[] = {"/bin/mkdir", "1 2 3 4", NULL};
	if (execve("/bin/mkdir", args, environ) < 0) {
		perror("execve");
		return 1;
	}
	
	return 0; // This line will not be reached if execve is successful
}
