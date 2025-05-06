#include "../../includes/minishell.h"

#include <stdbool.h>

// Function to match a string against a pattern
bool matchPattern(const char *str, const char *pattern) {
    // Base cases
    if (*pattern == '\0' && *str == '\0') return true; // Both exhausted
    if (*pattern == '\0') return false;               // Pattern exhausted, string not
    if (*str == '\0' && *pattern != '*') return false; // String exhausted, pattern not '*'

    // Handle literal characters
    if (*pattern != '*') {
        if (*str == *pattern || (*pattern == '.' && *str == '.')) {
            return matchPattern(str + 1, pattern + 1);
        }
        return false;
    }

    // Handle '*' (try matching zero or more characters)
    while (true) {
        if (matchPattern(str, pattern + 1)) return true; // Try skipping '*' (zero characters)
        if (*str == '\0') break;                        // No more characters to consume
        str++;                                          // Consume one character and try again
    }

    return false;
}

// Function to check if a filename matches "b*g*n*.txt"
bool isMatch(const char *filename) {
    const char *pattern = "b*g*n*.txt";
    return matchPattern(filename, pattern);
}


t_token_lst	*wildcard_function_if(t_wildcard_type_string *wildcard_string,
	t_token_lst *wildcard_list, t_read_dir *read_dir)
{
if (wildcard_string->type == WILDCARD_START)
	wildcard_list = join_wildcar_token(read_dir,
			wildcard_string, ft_strstr_last);
else if (wildcard_string->type == WILDCARD_END)
	wildcard_list = join_wildcar_token(read_dir,
			wildcard_string, ft_strstr_first);
else if (wildcard_string->type == WILDCARD_ONLY)
	wildcard_list = join_wildcar_token(read_dir,
			wildcard_string, NULL);
return (wildcard_list);
}


t_token_lst	*handle_wildcard(char *line)
{
	t_read_dir				read_dir;
	t_token_lst				*wildcard_list;
	t_wildcard_type_string	*wildcard_string;

	// wildcard_string = identify_wildcard_type(line);
	wildcard_list = NULL;
	read_dir.dir = opendir(".");
	if (read_dir.dir == NULL)
	{
		perror("opendir");
		return (NULL);
	}
	// wildcard_list = wildcard_function_if(wildcard_string,
	// 		wildcard_list, &read_dir);
	if (closedir(read_dir.dir))
	{
		perror("closedir");
		return (NULL);
	}
	if (wildcard_string->data)
		free(wildcard_string->data);
	free(wildcard_string);
	return (wildcard_list);
}
