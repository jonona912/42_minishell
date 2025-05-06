#include "../../includes/minishell.h"

// #include <stdbool.h>

// Function to match a string against a pattern
int match_pattern(const char *str, const char *pattern) {
    // Base cases
    if (*pattern == '\0' && *str == '\0') return 1; // Both exhausted
    if (*pattern == '\0') return 0;               // Pattern exhausted, string not
    if (*str == '\0' && *pattern != '*') return 0; // String exhausted, pattern not '*'

    // Handle literal characters
    if (*pattern != '*') {
        if (*str == *pattern || (*pattern == '.' && *str == '.')) {
            return match_pattern(str + 1, pattern + 1);
        }
        return 0;
    }

    // Handle '*' (try matching zero or more characters)
    while (1) {
		while (*pattern == '*') pattern++; // Skip consecutive '*'
        if (match_pattern(str, pattern)) return 1; // Try skipping '*' (zero characters)
        if (*str == '\0') break;                        // No more characters to consume
        str++;                                          // Consume one character and try again
    }

    return 0;
}

// Function to check if a filename matches "b*g*n*.txt"
int is_match(const char *filename, const char *pattern) {
    // const char *pattern = "b*g*n*.txt";
    return match_pattern(filename, pattern);
}

t_token_lst	*handle_wildcard_2(char *line)
{
	t_read_dir				read_dir;
	t_token_lst				*wildcard_list;
	// t_wildcard_type_string	*wildcard_string;

	// wildcard_string = identify_wildcard_type(line);
	wildcard_list = NULL;
	read_dir.dir = opendir(".");
	if (read_dir.dir == NULL)
	{
		perror("opendir");
		return (NULL);
	}
	while ((read_dir.entry = readdir(read_dir.dir)) != NULL) {
		if (is_match(read_dir.entry->d_name, line)) {
			token_add_node_back(&wildcard_list, token_new_node(TOKEN_WORD, ft_strdup(read_dir.entry->d_name)));
		}
    }
	if (closedir(read_dir.dir))
	{
		perror("closedir");
		return (NULL);
	}
	return (wildcard_list);
}
