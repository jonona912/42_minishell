#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_TOKENS 32
#define MAX_COMMANDS 10

// Tokenize input into tokens (including handling quotes and operators)
void tokenize(const char *input, char **tokens, int *token_count) {
    char current_token[256] = {0};
    int token_pos = 0;
    bool in_quotes = false, in_single_quotes = false, escape_next = false;

    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];

        if (escape_next) {
            current_token[token_pos++] = c;
            escape_next = false;
            continue;
        }

        if (c == '\\') {
            escape_next = true;
            continue;
        }

        if (c == '\"' && !in_single_quotes) in_quotes = !in_quotes;
        else if (c == '\'' && !in_quotes) in_single_quotes = !in_single_quotes;
        else if (strchr("|><&", c) && !in_quotes && !in_single_quotes) {
            if (token_pos > 0) {
                tokens[(*token_count)++] = strdup(current_token);
                memset(current_token, 0, sizeof(current_token));
                token_pos = 0;
            }
            tokens[(*token_count)++] = strndup(&c, 1);
        }
        else if (isspace(c) && !in_quotes && !in_single_quotes) {
            if (token_pos > 0) {
                tokens[(*token_count)++] = strdup(current_token);
                memset(current_token, 0, sizeof(current_token));
                token_pos = 0;
            }
        }
        else {
            current_token[token_pos++] = c;
        }
    }

    if (token_pos > 0) {
        tokens[(*token_count)++] = strdup(current_token);
    }
}

// Split tokens into individual commands separated by pipes
int split_commands(char **tokens, int token_count, char ***commands) {
    int cmd_idx = 0, arg_idx = 0;
    commands[cmd_idx] = malloc(MAX_TOKENS * sizeof(char *));

    for (int i = 0; i < token_count; i++) {
        if (strcmp(tokens[i], "|") == 0) {
            commands[cmd_idx][arg_idx] = NULL;
            cmd_idx++;
            arg_idx = 0;
            commands[cmd_idx] = malloc(MAX_TOKENS * sizeof(char *));
        } else {
            commands[cmd_idx][arg_idx++] = tokens[i];
        }
    }
    commands[cmd_idx][arg_idx] = NULL;
    return cmd_idx + 1;
}

// Find full path of executable using PATH
char *find_executable(const char *file) {
    char *path = getenv("PATH");
    if (!path) return NULL;

    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");

    while (dir) {
        char *full_path = malloc(strlen(dir) + strlen(file) + 2);
        sprintf(full_path, "%s/%s", dir, file);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

int main() {
    char *tokens[MAX_TOKENS];
    int token_count = 0;
    const char *input = "ls -l | grep get | wc -wc";

    // Tokenize input
    tokenize(input, tokens, &token_count);

	// Split into commands
	char *commands[MAX_COMMANDS][MAX_TOKENS];
	int num_commands = split_commands(tokens, token_count, commands);

	// Create pipes
	int pipes[num_commands-1][2];
	for (int i = 0; i < num_commands-1; i++) {
		pipe(pipes[i]);
	}

    // Execute commands
    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid == 0) {
			// Connect pipes
			if (i > 0) {
				dup2(pipes[i-1][0], STDIN_FILENO);
			}
			if (i < num_commands-1) {
				dup2(pipes[i][1], STDOUT_FILENO);
			}

			// Close all pipe ends
			for (int j = 0; j < num_commands-1; j++) {
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			// Prepare arguments
			char *argv[MAX_TOKENS];
			int arg_idx = 0;
			while (commands[i][arg_idx]) {
				argv[arg_idx] = commands[i][arg_idx];
				arg_idx++;
			}
			argv[arg_idx] = NULL;

			// Execute command
			char *full_path = find_executable(argv[0]);
			if (!full_path) {
				fprintf(stderr, "Command not found: %s\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			execve(full_path, argv, NULL);
			exit(EXIT_FAILURE);
        }
    }

    // Close all pipes in parent
    for (int i = 0; i < num_commands-1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }

    // Cleanup
    for (int i = 0; i < token_count; i++) free(tokens[i]);
    for (int i = 0; i < num_commands; i++) free(commands[i]);

    return 0;
}