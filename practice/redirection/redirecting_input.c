#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	char c;
	pid_t pid;
	int pipefd[2];
	// Redirecting input from a file
	int fd = open("example.txt", O_RDONLY);
	int fd_result = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_result == -1 || fd == -1)
	{
		perror("open");
		return 1;
	}
	if (pipe(pipefd) == -1) {
		perror("pipe");
		return 1;
	}
	// pipefd[0] is the read end, pipefd[1] is the write end
	pid = fork();
	if (pid == -1) {
		perror("fork");
		return 1;
	}
	if (pid == 0)
	{ // Child process
		// Redirect standard input to the file
		if (dup2(fd, 0) == -1) {
			perror("dup2");
			return 1;
		}
		close(fd); // Close the file descriptor
		if (dup2(fd_result, 1) == -1) {
			perror("dup2");
			return 1;
		}
		close(fd_result); // Close the file descriptor
		close(fd); // Close the file descriptor
		execve("/bin/sort", (char *[]){"sort", NULL}, NULL); 
	}
	wait(NULL); // Wait for the child process to finish
	printf("Parent process: %d\n", getpid());
	return (0);
}
