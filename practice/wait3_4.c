// // wait3
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/wait.h>
// #include <sys/resource.h>
// #include <unistd.h>

// int main() {
//     pid_t pid = fork();

//     if (pid == 0) {
//         // Child process: execute a command or do work
//         printf("Child process (PID: %d)\n", getpid());
//         sleep(1); // Simulate work
//         exit(42); // Exit with status 42
//     } else if (pid > 0) {
//         // Parent process
//         int status;
//         struct rusage usage;
//         pid_t child_pid = wait3(&status, 0, &usage); // Wait for any child

//         if (child_pid == -1) {
//             perror("wait3");
//             exit(EXIT_FAILURE);
//         }

//         if (WIFEXITED(status)) {
//             printf("Child %d exited with status: %d\n", child_pid, WEXITSTATUS(status));
//         }

//         // Print resource usage
//         printf("User CPU time: %ld.%06ld seconds\n",
//                usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
//         printf("System CPU time: %ld.%06ld seconds\n",
//                usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
//     } else {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }

//     return 0;
// }










// wait 4
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        printf("Child process (PID: %d)\n", getpid());
        sleep(1);
        exit(42);
    } else if (pid > 0) {
        // Parent process
        int status;
        struct rusage usage;
        // Wait for a specific child using its PID
        pid_t child_pid = wait4(pid, &status, 0, &usage);

        if (child_pid == -1) {
            perror("wait4");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            printf("Child %d exited with status: %d\n", child_pid, WEXITSTATUS(status));
        }

        // Print resource usage (same as wait3)
        printf("User CPU time: %ld.%06ld seconds\n",
               usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
        printf("System CPU time: %ld.%06ld seconds\n",
               usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}


