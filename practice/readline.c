// #include <stdio.h>
// #include <stdlib.h>
// #include <readline/readline.h>
// #include <readline/history.h>

// int main() {
//     char *input;
    
//     while (1) {
//         // Display prompt and read input
//         input = readline("Enter command: ");
        
//         // Exit on EOF or empty input
//         if (!input || *input == '\0') {
//             free(input);
//             break;
//         }
        
//         // Add to history
//         add_history(input);
        
//         // Process the input...
//         printf("You entered: %s\n", input);
//         // Free allocated memory
//         free(input);
//     }
//     return 0;
// }

#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

void sigint_handler(int sig) {
    printf("Interrupt\n");
    rl_on_new_line();  // Inform Readline that the cursor is on a new line
    rl_redisplay();  // Redraw the input line without informing Readline of the new line
}

int main() {
    rl_catch_signals = 0;  // Disable Readline's default signal handling
    signal(SIGINT, sigint_handler);
    char *line;
    while ((line = readline("Prompt> ")) != NULL) {
        printf("You entered: %s\n", line);
        add_history(line);
        free(line);
    }
    return 0;
}
