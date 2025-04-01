#define _POSIX_C_SOURCE 200809L
// #include <stdio.h>
// #include <signal.h>
// #include <unistd.h>

// void handler(int sig) {
//     if (sig == SIGINT) {
//         printf("Caught Ctrl+C (SIGINT) %d\n", sig);
//     } else if (sig == SIGQUIT) {
//         printf("Caught Ctrl+\\ (SIGQUIT) %d\n", sig);
//     }
// }

// int main() {
//     struct sigaction sa;
//     sa.sa_handler = handler;
//     sigemptyset(&sa.sa_mask);
//     sa.sa_flags = 0;

//     // Register handler for SIGINT (Ctrl+C)
//     sigaction(SIGINT, &sa, NULL);
// 	sleep(3);
//     // Register same handler for SIGQUIT (Ctrl+\)
//     sigaction(SIGQUIT, &sa, NULL);

//     while(1) pause();  // Wait indefinitely for signals
//     return 0;
// }



#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Custom signal handler for SIGINT (Ctrl+C)
void sigint_handler(int sig) {
    const char *msg = "\nCaught SIGINT! (But I won't die)\n";
    write(STDOUT_FILENO, msg, strlen(msg));
}

int main() {
    struct sigaction sa;

    // Step 1: Clear the signal set (sa.sa_mask) using sigemptyset
    sigemptyset(&sa.sa_mask);  // No additional signals to block inside handler

    // Step 2: Set up the handler
    sa.sa_handler = sigint_handler;  // Use our custom handler
    sa.sa_flags = 0;  // No special flags

    // Step 3: Register the handler for SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction failed");
        return 1;
    }

    printf("Press Ctrl+C to test. (I'll ignore it!)\n");
    printf("Press Ctrl+\\ (SIGQUIT) to terminate.\n");

    // Keep the program running to test the handler
    while (1) {
        sleep(1);
    }

    return 0;
}