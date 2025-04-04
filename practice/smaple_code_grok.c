#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// Structure to represent a command in the pipeline
typedef struct {
    char **argv;        // Command and arguments (null-terminated)
    int argc;           // Number of arguments
    char *input_redir;  // Input redirection file, or NULL
    char *output_redir; // Output redirection file, or NULL
} Command;

int main() {
    // Hardcoded command for this example
    char *cmd = "cat < input.txt | grep sth | wc -w > output.txt";

    // --- Step 1: Tokenization ---
    // Make a copy of the command since strtok modifies the string
    char *cmd_copy = strdup(cmd);
    char *tokens[100];  // Array to hold tokens (max 100 for simplicity)
    int token_count = 0;

    // Split the command string by spaces
    char *token = strtok(cmd_copy, " ");
    while (token != NULL) {
        tokens[token_count++] = strdup(token);
        token = strtok(NULL, " ");
    }

    // --- Step 2: Parsing ---
    Command commands[10];  // Array to hold parsed commands (max 10)
    int cmd_index = 0;     // Number of commands in the pipeline
    int i = 0;             // Index into tokens array

    // Parse tokens into commands separated by '|'
    while (i < token_count) {
        Command *cmd = &commands[cmd_index++];
        cmd->argv = malloc(100 * sizeof(char*));  // Max 100 args per command
        cmd->argc = 0;
        cmd->input_redir = NULL;
        cmd->output_redir = NULL;

        // Collect tokens for the current command until '|' or end
        while (i < token_count && strcmp(tokens[i], "|") != 0) {
            if (strcmp(tokens[i], "<") == 0) {
                i++;
                cmd->input_redir = strdup(tokens[i]);
            } else if (strcmp(tokens[i], ">") == 0) {
                i++;
                cmd->output_redir = strdup(tokens[i]);
            } else {
                cmd->argv[cmd->argc++] = strdup(tokens[i]);
            }
            i++;
        }
        cmd->argv[cmd->argc] = NULL;  // Null-terminate argv

        if (i < token_count && strcmp(tokens[i], "|") == 0) {
            i++;  // Skip the '|' token
        }
    }
    int num_commands = cmd_index;

    // --- Step 3: Execution ---
    int prev_pipe_read = -1;  // Read end of the previous pipe

    // Iterate through each command in the pipeline
    for (int i = 0; i < num_commands; i++) {
        int pipefd[2];  // Pipe file descriptors

        // Create a pipe if this is not the last command
        if (i < num_commands - 1) {
            if (pipe(pipefd) == -1) {
                perror("pipe");
                exit(1);
            }
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {  // Child process
            // Set up stdin
            if (i == 0 && commands[i].input_redir) {
                // First command with input redirection
                int fd_in = open(commands[i].input_redir, O_RDONLY);
                if (fd_in == -1) {
                    perror("open input_redir");
                    exit(1);
                }
                dup2(fd_in, 0);  // Redirect stdin to the file
                close(fd_in);
            } else if (i > 0) {
                // Middle or last command: stdin from previous pipe
                dup2(prev_pipe_read, 0);
                close(prev_pipe_read);
            }

            // Set up stdout
            if (i == num_commands - 1 && commands[i].output_redir) {
                // Last command with output redirection
                int fd_out = open(commands[i].output_redir, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd_out == -1) {
                    perror("open output_redir");
                    exit(1);
                }
                dup2(fd_out, 1);  // Redirect stdout to the file
                close(fd_out);
            } else if (i < num_commands - 1) {
                // First or middle command: stdout to pipe
                dup2(pipefd[1], 1);
                close(pipefd[1]);
            }

            // Close remaining pipe ends
            if (i < num_commands - 1) {
                close(pipefd[0]);
            }
            if (prev_pipe_read != -1) {
                close(prev_pipe_read);
            }

            // Execute the command
            execvp(commands[i].argv[0], commands[i].argv);
            perror("execvp");  // If execvp returns, it failed
            exit(1);
        }

        // Parent process
        if (i > 0) {
            close(prev_pipe_read);  // Close previous pipe's read end
        }
        if (i < num_commands - 1) {
            close(pipefd[1]);       // Close current pipe's write end
            prev_pipe_read = pipefd[0];  // Save read end for next command
        }
    }

    // Close the last pipe's read end in the parent
    if (num_commands > 1) {
        close(prev_pipe_read);
    }

    // Wait for all child processes to complete
    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }

    // Free allocated memory (simplified; in practice, free all strdup'd strings)
    free(cmd_copy);
    for (int i = 0; i < token_count; i++) {
        free(tokens[i]);
    }
    for (int i = 0; i < num_commands; i++) {
        for (int j = 0; j < commands[i].argc; j++) {
            free(commands[i].argv[j]);
        }
        free(commands[i].argv);
        if (commands[i].input_redir) free(commands[i].input_redir);
        if (commands[i].output_redir) free(commands[i].output_redir);
    }

    return 0;
}