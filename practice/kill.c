#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Child process
        printf("Child running (PID: %d)\n", getpid());
        while (1) {}  // Infinite loop
    } else {
        // Parent process
        printf("Parent sending SIGTERM to child (PID: %d)\n", child_pid);
        sleep(2);  // Give child time to start
        kill(child_pid, SIGTERM);  // Terminate child
        wait(NULL);  // Reap child
        printf("Child terminated\n");
    }

    return 0;
}