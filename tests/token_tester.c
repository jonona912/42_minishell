#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../minishell_files/includes/minishell.h"

// Color definitions
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define RESET   "\x1B[0m"

typedef struct s_test_case {
    char *input;
    char *expected_output;
} t_test_case;

// Replace this with your actual tokenizer function declaration
t_token_lst *ft_tokenize(char *line);

// Test cases - add more as needed
t_test_case test_cases[] = {
    // Basic commands
    {"ls",
        "Type : TOKEN_WORD\nToken: \"ls\"\n"},
    {"echo hello",
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_WORD\nToken: \"hello\"\n"},
    {"ls -la",
        "Type : TOKEN_WORD\nToken: \"ls\"\n"
        "Type : TOKEN_WORD\nToken: \"-la\"\n"},
    {"grep --color=auto -n \"pattern\"",
        "Type : TOKEN_WORD\nToken: \"grep\"\n"
        "Type : TOKEN_WORD\nToken: \"--color=auto\"\n"
        "Type : TOKEN_WORD\nToken: \"-n\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"pattern\"\n"},
    {"/usr/bin/ls -l /home/user",
        "Type : TOKEN_WORD\nToken: \"/usr/bin/ls\"\n"
        "Type : TOKEN_WORD\nToken: \"-l\"\n"
        "Type : TOKEN_WORD\nToken: \"/home/user\"\n"},
    {"cd ../..",
        "Type : TOKEN_WORD\nToken: \"cd\"\n"
        "Type : TOKEN_WORD\nToken: \"../..\"\n"},

    //error
    {"echo hello$USER",
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_WORD\nToken: \"hello\"\n"
        "Type : TOKEN_ENV_VAR\nToken: \"$USER\"\n"},

    {"touch file{1..3}.txt",
        "Type : TOKEN_WORD\nToken: \"touch\"\n"
        "Type : TOKEN_WORD\nToken: \"file{1..3}.txt\"\n"},
    {"ls; pwd; date",
    "Type : TOKEN_WORD\nToken: \"ls;\"\n"
    "Type : TOKEN_WORD\nToken: \"pwd;\"\n"
    "Type : TOKEN_WORD\nToken: \"date\"\n"},
    {"make && make install",
    "Type : TOKEN_WORD\nToken: \"make\"\n"
    "Type : TOKEN_AND\nToken: \"&&\"\n"
    "Type : TOKEN_WORD\nToken: \"make\"\n"
    "Type : TOKEN_WORD\nToken: \"install\"\n"},
    {"git commit -m \"Initial commit\"",
        "Type : TOKEN_WORD\nToken: \"git\"\n"
        "Type : TOKEN_WORD\nToken: \"commit\"\n"
        "Type : TOKEN_WORD\nToken: \"-m\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"Initial commit\"\n"},
    {"docker run -it --rm ubuntu bash",
        "Type : TOKEN_WORD\nToken: \"docker\"\n"
        "Type : TOKEN_WORD\nToken: \"run\"\n"
        "Type : TOKEN_WORD\nToken: \"-it\"\n"
        "Type : TOKEN_WORD\nToken: \"--rm\"\n"
        "Type : TOKEN_WORD\nToken: \"ubuntu\"\n"
        "Type : TOKEN_WORD\nToken: \"bash\"\n"},
    {"\t\nls\t-l\n",  // Whitespace handling
    "Type : TOKEN_WORD\nToken: \"ls\"\n"
    "Type : TOKEN_WORD\nToken: \"-l\"\n"},
    {"echo hello\\ world",  // Escaped space
    "Type : TOKEN_WORD\nToken: \"echo\"\n"
    "Type : TOKEN_WORD\nToken: \"hello\\\"\n"
    "Type : TOKEN_WORD\nToken: \"world\"\n"},
    {"PATH=/usr/bin ls -l | grep \"\\.c\" > c_files.txt",
        "Type : TOKEN_WORD\nToken: \"PATH=/usr/bin\"\n"
        "Type : TOKEN_WORD\nToken: \"ls\"\n"
        "Type : TOKEN_WORD\nToken: \"-l\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
        "Type : TOKEN_WORD\nToken: \"grep\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"\\.c\"\n"
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
        "Type : TOKEN_WORD\nToken: \"c_files.txt\"\n"},
    {"(cd src && make) || echo \"Build failed\"",
        "Type : TOKEN_L_PAREN\nToken: \"(\"\n"
        "Type : TOKEN_WORD\nToken: \"cd\"\n"
        "Type : TOKEN_WORD\nToken: \"src\"\n"
        "Type : TOKEN_AND\nToken: \"&&\"\n"
        "Type : TOKEN_WORD\nToken: \"make\"\n"
        "Type : TOKEN_R_PAREN\nToken: \")\"\n"
        "Type : TOKEN_OR\nToken: \"||\"\n"
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"Build failed\"\n"},

    // Quotes
    {"echo \"hello\"",
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"hello\"\n"},
    {"echo 'world'",
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_S_QUOTE\nToken: \"world\"\n"},
    {"echo \"double 'single' quotes\"",
    "Type : TOKEN_WORD\nToken: \"echo\"\n"
    "Type : TOKEN_D_QUOTE\nToken: \"double 'single' quotes\"\n"},
    {"echo 'double \"quotes\" inside'",
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_S_QUOTE\nToken: \"double \"quotes\" inside\"\n"},
    {"echo 'hello''world'",
    "Type : TOKEN_WORD\nToken: \"echo\"\n"
    "Type : TOKEN_S_QUOTE\nToken: \"helloworld\"\n"},
    {"echo \"hello\"\"world\"",
    "Type : TOKEN_WORD\nToken: \"echo\"\n"
    "Type : TOKEN_D_QUOTE\nToken: \"helloworld\"\n"},
    {"echo \"$PATH contains paths\"",
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"$PATH contains paths\"\n"},
    {"echo '$(date)'",
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_S_QUOTE\nToken: \"$(date)\"\n"},
    {"echo \"Command 'date' says: $(date)\"",
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"Command 'date' says: $(date)\"\n"},
    {"echo 'Nested \"quotes\" work'",
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_S_QUOTE\nToken: \"Nested \"quotes\" work\"\n"},
    {"echo \"\"",  // Empty double quotes
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"\"\n"},
    {"echo ''",  // Empty single quotes (I'm not sure)
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_S_QUOTE\nToken: \"\"\n"},
    {"echo \"   \"",  // Spaces in quotes
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"   \"\n"},
    {"echo \"Today's date is '$(date)' and PATH is $PATH\" | tee log.txt",
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"Today's date is '$(date)' and PATH is $PATH\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
        "Type : TOKEN_WORD\nToken: \"tee\"\n"
        "Type : TOKEN_WORD\nToken: \"log.txt\"\n"},
    //errors
    // {"echo \"unclosed quote",
    //     "Type : TOKEN_WORD\nToken: \"echo\"\n"
    //     "Type : TOKEN_D_QUOTE\nToken: \"unclosed quote\"\n"
    //     "Error: unmatched quotes\n"
    // },
    // {"echo 'unclosed single quote",
    //     "Type : TOKEN_WORD\nToken: \"echo\"\n"
    //     "Type : TOKEN_S_QUOTE\nToken: \"unclosed single quote\"\n"
    //     "Error: unmatched quotes\n"
    // },
    // {"echo \"bad' quote\"",
    //     "Type : TOKEN_WORD\nToken: \"echo\"\n"
    //     "Error: mixed quotes in single token\n"
    // },


    // Redirections
    {"cat < file",
        "Type : TOKEN_WORD\nToken: \"cat\"\n"
        "Type : TOKEN_REDIRECTION_IN\nToken: \"<\"\n"
        "Type : TOKEN_WORD\nToken: \"file\"\n"},
    {"sort < input.txt > output.txt",
        "Type : TOKEN_WORD\nToken: \"sort\"\n"
        "Type : TOKEN_REDIRECTION_IN\nToken: \"<\"\n"
        "Type : TOKEN_WORD\nToken: \"input.txt\"\n"
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
        "Type : TOKEN_WORD\nToken: \"output.txt\"\n"},
    {"echo hello >> log.txt",
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_WORD\nToken: \"hello\"\n"
        "Type : TOKEN_APPEND\nToken: \">>\"\n"
        "Type : TOKEN_WORD\nToken: \"log.txt\"\n"},
    {"cat << EOF",
        "Type : TOKEN_WORD\nToken: \"cat\"\n"
        "Type : TOKEN_HEREDOC\nToken: \"<<\"\n"
        "Type : TOKEN_WORD\nToken: \"EOF\"\n"},
    {"grep error < log.txt | awk '{print $1}' > errors.txt",
    "Type : TOKEN_WORD\nToken: \"grep\"\n"
    "Type : TOKEN_WORD\nToken: \"error\"\n"
    "Type : TOKEN_REDIRECTION_IN\nToken: \"<\"\n"
    "Type : TOKEN_WORD\nToken: \"log.txt\"\n"
    "Type : TOKEN_PIPE\nToken: \"|\"\n"
    "Type : TOKEN_WORD\nToken: \"awk\"\n"
    "Type : TOKEN_S_QUOTE\nToken: \"{print $1}\"\n"
    "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
    "Type : TOKEN_WORD\nToken: \"errors.txt\"\n"},
    {"(ls -l > file_list.txt) && cat file_list.txt",
        "Type : TOKEN_L_PAREN\nToken: \"(\"\n"
        "Type : TOKEN_WORD\nToken: \"ls\"\n"
        "Type : TOKEN_WORD\nToken: \"-l\"\n"
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
        "Type : TOKEN_WORD\nToken: \"file_list.txt\"\n"
        "Type : TOKEN_R_PAREN\nToken: \")\"\n"
        "Type : TOKEN_AND\nToken: \"&&\"\n"
        "Type : TOKEN_WORD\nToken: \"cat\"\n"
        "Type : TOKEN_WORD\nToken: \"file_list.txt\"\n"},
    {"< input.txt cat",  // Redirection before command
        "Type : TOKEN_REDIRECTION_IN\nToken: \"<\"\n"
        "Type : TOKEN_WORD\nToken: \"input.txt\"\n"
        "Type : TOKEN_WORD\nToken: \"cat\"\n"},
    {"cat >",  // Missing filename
        "Type : TOKEN_WORD\nToken: \"cat\"\n"
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"},
    {"echo hello > file1 > file2",  // Multiple outputs
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_WORD\nToken: \"hello\"\n"
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
        "Type : TOKEN_WORD\nToken: \"file1\"\n"
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
        "Type : TOKEN_WORD\nToken: \"file2\"\n"},


    // Pipes
    {"ls | grep .c", "Type : TOKEN_WORD\nToken: \"ls\"\nType : TOKEN_PIPE\nToken: \"|\"\nType : TOKEN_WORD\nToken: \"grep\"\nType : TOKEN_WORD\nToken: \".c\"\n"},
    {"cat file | sort | uniq | wc -l",
        "Type : TOKEN_WORD\nToken: \"cat\"\n"
        "Type : TOKEN_WORD\nToken: \"file\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
        "Type : TOKEN_WORD\nToken: \"sort\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
        "Type : TOKEN_WORD\nToken: \"uniq\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
        "Type : TOKEN_WORD\nToken: \"wc\"\n"
        "Type : TOKEN_WORD\nToken: \"-l\"\n"},
    {"ls -l | grep test > output.txt",
        "Type : TOKEN_WORD\nToken: \"ls\"\n"
        "Type : TOKEN_WORD\nToken: \"-l\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
        "Type : TOKEN_WORD\nToken: \"grep\"\n"
        "Type : TOKEN_WORD\nToken: \"test\"\n"
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
        "Type : TOKEN_WORD\nToken: \"output.txt\"\n"},
    {"echo \"hello world\" | awk '{print $1}'",
    "Type : TOKEN_WORD\nToken: \"echo\"\n"
    "Type : TOKEN_D_QUOTE\nToken: \"hello world\"\n"
    "Type : TOKEN_PIPE\nToken: \"|\"\n"
    "Type : TOKEN_WORD\nToken: \"awk\"\n"
    "Type : TOKEN_S_QUOTE\nToken: \"{print $1}\"\n"},
    {"(ls | grep .c) && echo found",
        "Type : TOKEN_L_PAREN\nToken: \"(\"\n"
        "Type : TOKEN_WORD\nToken: \"ls\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
        "Type : TOKEN_WORD\nToken: \"grep\"\n"
        "Type : TOKEN_WORD\nToken: \".c\"\n"
        "Type : TOKEN_R_PAREN\nToken: \")\"\n"
        "Type : TOKEN_AND\nToken: \"&&\"\n"
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_WORD\nToken: \"found\"\n"},
    {"cat access.log | grep 404 | awk '{print $7}' | sort | uniq -c | sort -nr",
    "Type : TOKEN_WORD\nToken: \"cat\"\n"
    "Type : TOKEN_WORD\nToken: \"access.log\"\n"
    "Type : TOKEN_PIPE\nToken: \"|\"\n"
    "Type : TOKEN_WORD\nToken: \"grep\"\n"
    "Type : TOKEN_WORD\nToken: \"404\"\n"
    "Type : TOKEN_PIPE\nToken: \"|\"\n"
    "Type : TOKEN_WORD\nToken: \"awk\"\n"
    "Type : TOKEN_S_QUOTE\nToken: \"{print $7}\"\n"
    "Type : TOKEN_PIPE\nToken: \"|\"\n"
    "Type : TOKEN_WORD\nToken: \"sort\"\n"
    "Type : TOKEN_PIPE\nToken: \"|\"\n"
    "Type : TOKEN_WORD\nToken: \"uniq\"\n"
    "Type : TOKEN_WORD\nToken: \"-c\"\n"
    "Type : TOKEN_PIPE\nToken: \"|\"\n"
    "Type : TOKEN_WORD\nToken: \"sort\"\n"
    "Type : TOKEN_WORD\nToken: \"-nr\"\n"},
    {"| ls",  // Leading pipe
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
        "Type : TOKEN_WORD\nToken: \"ls\"\n"},
    {"ls |",  // Trailing pipe
        "Type : TOKEN_WORD\nToken: \"ls\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"},
    {"ls || grep",  // Double pipe
        "Type : TOKEN_WORD\nToken: \"ls\"\n"
        "Type : TOKEN_OR\nToken: \"||\"\n"
        "Type : TOKEN_WORD\nToken: \"grep\"\n"},
    {"ls|grep",  // No spaces
        "Type : TOKEN_WORD\nToken: \"ls\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
        "Type : TOKEN_WORD\nToken: \"grep\"\n"},
    {"(find . -name '*.c' | xargs wc -l | sort -nr) > line_counts.txt && echo Done",
        "Type : TOKEN_L_PAREN\nToken: \"(\"\n"
        "Type : TOKEN_WORD\nToken: \"find\"\n"
        "Type : TOKEN_WORD\nToken: \".\"\n"
        "Type : TOKEN_WORD\nToken: \"-name\"\n"
        "Type : TOKEN_S_QUOTE\nToken: \"*.c\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
        "Type : TOKEN_WORD\nToken: \"xargs\"\n"
        "Type : TOKEN_WORD\nToken: \"wc\"\n"
        "Type : TOKEN_WORD\nToken: \"-l\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
        "Type : TOKEN_WORD\nToken: \"sort\"\n"
        "Type : TOKEN_WORD\nToken: \"-nr\"\n"
        "Type : TOKEN_R_PAREN\nToken: \")\"\n"
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
        "Type : TOKEN_WORD\nToken: \"line_counts.txt\"\n"
        "Type : TOKEN_AND\nToken: \"&&\"\n"
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_WORD\nToken: \"Done\"\n"},

    // Parentheses
    {"(echo hello)", "Type : TOKEN_L_PAREN\nToken: \"(\"\nType : TOKEN_WORD\nToken: \"echo\"\nType : TOKEN_WORD\nToken: \"hello\"\nType : TOKEN_R_PAREN\nToken: \")\"\n"},
    {"(echo hello; echo world)", "Type : TOKEN_L_PAREN\nToken: \"(\"\nType : TOKEN_WORD\nToken: \"echo\"\nType : TOKEN_WORD\nToken: \"hello;\"\nType : TOKEN_WORD\nToken: \"echo\"\nType : TOKEN_WORD\nToken: \"world\"\nType : TOKEN_R_PAREN\nToken: \")\"\n"},
    {"((echo hello))",
        "Type : TOKEN_L_PAREN\nToken: \"(\"\n"
        "Type : TOKEN_L_PAREN\nToken: \"(\"\n"
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_WORD\nToken: \"hello\"\n"
        "Type : TOKEN_R_PAREN\nToken: \")\"\n"
        "Type : TOKEN_R_PAREN\nToken: \")\"\n"},
    {"(echo hello && echo world)",
        "Type : TOKEN_L_PAREN\nToken: \"(\"\n"
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_WORD\nToken: \"hello\"\n"
        "Type : TOKEN_AND\nToken: \"&&\"\n"
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_WORD\nToken: \"world\"\n"
        "Type : TOKEN_R_PAREN\nToken: \")\"\n"},
    {"(cat < input.txt) > output.txt",
        "Type : TOKEN_L_PAREN\nToken: \"(\"\n"
        "Type : TOKEN_WORD\nToken: \"cat\"\n"
        "Type : TOKEN_REDIRECTION_IN\nToken: \"<\"\n"
        "Type : TOKEN_WORD\nToken: \"input.txt\"\n"
        "Type : TOKEN_R_PAREN\nToken: \")\"\n"
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
        "Type : TOKEN_WORD\nToken: \"output.txt\"\n"},
    {"()",
        "Type : TOKEN_L_PAREN\nToken: \"(\"\n"
        "Type : TOKEN_R_PAREN\nToken: \")\"\n"},
    {"( )",
        "Type : TOKEN_L_PAREN\nToken: \"(\"\n"
        "Type : TOKEN_R_PAREN\nToken: \")\"\n"},
    {"(echo)",
        "Type : TOKEN_L_PAREN\nToken: \"(\"\n"
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_R_PAREN\nToken: \")\"\n"},

    //error cases parenthesis
    {"(echo hello",
        ""
    },
    {"echo hello)",
        ""
    },
    {"((echo hello)",
        ""
    },

    // Edge cases
    {"", ""},  // Empty input
    {"   ", ""},  // Whitespace only
    {"\t\n\v\f\r ", ""},  // All whitespace characters
    {"a",  // Single character
        "Type : TOKEN_WORD\nToken: \"a\"\n"},
    {" a ",  // Single character with spaces
        "Type : TOKEN_WORD\nToken: \"a\"\n"},
    {"|",  // Lone pipe
        "Type : TOKEN_PIPE\nToken: \"|\"\n"},
    {"\\",  // Lone backslash
    "Type : TOKEN_WORD\nToken: \"\\\"\n"},
    {"echo \\",  // Trailing backslash
    "Type : TOKEN_WORD\nToken: \"echo\"\n"
    "Type : TOKEN_WORD\nToken: \"\\\"\n"},
    {"$",  // Lone dollar
    "Type : TOKEN_ENV_VAR\nToken: \"$\"\n"},
    {"cmd\targ",  // Tab separator
        "Type : TOKEN_WORD\nToken: \"cmd\"\n"
        "Type : TOKEN_WORD\nToken: \"arg\"\n"
    },
    {"cmd\narg",  // Newline separator
        "Type : TOKEN_WORD\nToken: \"cmd\"\n"
        "Type : TOKEN_WORD\nToken: \"arg\"\n"
    },
    {"cmd \t \n arg",  // Mixed whitespace
        "Type : TOKEN_WORD\nToken: \"cmd\"\n"
        "Type : TOKEN_WORD\nToken: \"arg\"\n"
    },
    {"\"\"",  // Empty double quotes
        "Type : TOKEN_D_QUOTE\nToken: \"\"\n"
    },
    {"''",  // Empty single quotes
        "Type : TOKEN_S_QUOTE\nToken: \"\"\n"
    },
    {"\" \"",  // Space in quotes
        "Type : TOKEN_D_QUOTE\nToken: \" \"\n"
    },
    {"'\\''",  // Escaped single quote in single quotes
        ""
    },
    {">",  // Lone redirection
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
    },
    {"2>",  // Lone FD redirection
        "Type : TOKEN_WORD\nToken: \"2\"\n"
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
    },
    {"<>file",  // Special redirection
        "Type : TOKEN_REDIRECTION_IN\nToken: \"<\"\n"
        "Type : TOKEN_REDIRECTION_OUT\nToken: \">\"\n"
        "Type : TOKEN_WORD\nToken: \"file\"\n"
    },
    {";;",  // Double semicolon
        "Type : TOKEN_WORD\nToken: \";;\"\n"
    },
    {"|||",  // Triple pipe
        "Type : TOKEN_OR\nToken: \"||\"\n"
        "Type : TOKEN_PIPE\nToken: \"|\"\n"
    },
    {"\"\"\"\"\"\"",  // Triple empty quotes
        "Type : TOKEN_D_QUOTE\nToken: \"\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"\"\n"
        "Type : TOKEN_D_QUOTE\nToken: \"\"\n"
    },
    {"$''",  // ANSI-C quoting (bash extension)
        "Type : TOKEN_ENV_VAR\nToken: \"$\"\n"
        "Type : TOKEN_S_QUOTE\nToken: \"\"\n"
    },
    {"'$'",  // ANSI-C quoting (bash extension)
        "Type : TOKEN_S_QUOTE\nToken: \"$\"\n"
    },
    {"\"$\"",  // ANSI-C quoting (bash extension)
        "Type : TOKEN_D_QUOTE\nToken: \"$\"\n"
    },
    {"echo \"hello",  // Unclosed quote
        ""
    },
    {"echo hello # comment",  // Comment handling
        "Type : TOKEN_WORD\nToken: \"echo\"\n"
        "Type : TOKEN_WORD\nToken: \"hello\"\n"
        "Type : TOKEN_WORD\nToken: \"#\"\n"
        "Type : TOKEN_WORD\nToken: \"comment\"\n"
    },
    {NULL, NULL} // End marker
};

// Function to convert token list to string (mockup - implement according to your token list structure)
char *token_list_to_str(t_token_lst *tokens)
{
    char *result = strdup("");
    t_token_lst *current = tokens;

    while (current)
    {
        char *temp;
        char type_str[50];

        // Convert type to string
        switch(current->type) {
            case TOKEN_WORD: strcpy(type_str, "TOKEN_WORD"); break;
            case TOKEN_PIPE: strcpy(type_str, "TOKEN_PIPE"); break;
            case TOKEN_REDIRECTION_IN: strcpy(type_str, "TOKEN_REDIRECTION_IN"); break;
            case TOKEN_REDIRECTION_OUT: strcpy(type_str, "TOKEN_REDIRECTION_OUT"); break;
            case TOKEN_APPEND: strcpy(type_str, "TOKEN_APPEND"); break;
            case TOKEN_HEREDOC: strcpy(type_str, "TOKEN_HEREDOC"); break;
            case TOKEN_D_QUOTE: strcpy(type_str, "TOKEN_D_QUOTE"); break;
            case TOKEN_S_QUOTE: strcpy(type_str, "TOKEN_S_QUOTE"); break;
            case TOKEN_L_PAREN: strcpy(type_str, "TOKEN_L_PAREN"); break;
            case TOKEN_R_PAREN: strcpy(type_str, "TOKEN_R_PAREN"); break;
            case TOKEN_ENV_VAR: strcpy(type_str, "TOKEN_ENV_VAR"); break;
            case TOKEN_AND: strcpy(type_str, "TOKEN_AND"); break;
            case TOKEN_OR: strcpy(type_str, "TOKEN_OR"); break;
            default: strcpy(type_str, "UNKNOWN_TYPE"); break;
        }

        // Portable string concatenation
        int needed = snprintf(NULL, 0, "%sType : %s\nToken: \"%s\"\n",
                             result, type_str, current->value);
        temp = malloc(needed + 1);
        if (temp) {
            sprintf(temp, "%sType : %s\nToken: \"%s\"\n",
                    result, type_str, current->value);
            free(result);
            result = temp;
        } else {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        current = current->next;
    }

    return result;
}

void run_tests()
{
    int passed = 0;
    int total = 0;

    printf(YELLOW "=== Starting Tokenizer Tests ===\n" RESET);

    for (int i = 0; test_cases[i].input != NULL; i++)
    {
        total++;
        printf(CYAN "\nTest %d: \"%s\"\n" RESET, i+1, test_cases[i].input);

        // Tokenize the input
        t_token_lst *tokens = ft_tokenize(test_cases[i].input);
        char *actual_output = token_list_to_str(tokens);

        // Compare with expected output
        if (strcmp(actual_output, test_cases[i].expected_output) == 0)
        {
            printf(GREEN "[PASSED]\n" RESET);
            passed++;
        }
        else
        {
            printf(RED "[FAILED]\n" RESET);
            printf(MAGENTA "Expected:\n%s" RESET, test_cases[i].expected_output);
            printf(RED "Actual:\n%s" RESET, actual_output);
        }

        // Clean up
        free(actual_output);
        token_free_list(tokens);
    }

    printf(YELLOW "\n=== Test Summary ===\n" RESET);
    printf("Passed: " GREEN "%d/%d\n" RESET, passed, total);
    printf("Coverage: %.1f%%\n", (float)passed/total * 100);
}

int main()
{
    run_tests();
    return 0;
}
