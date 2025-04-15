#include "../../includes/minishell.h"

#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>



t_token_lst *wildcard_function(char *wildcard_str)
{
	DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
	t_token_lst *wildcard_list = NULL;

	wildcard_str = NULL;
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }
    
    printf("Directory contents with types:\n");
    while ((entry = readdir(dir)) != NULL) {
        if (stat(entry->d_name, &file_stat)) {
            perror("stat");
            continue;
        }
		// write functions to handle beginning* and beg*end in libft
		if (ft_strstr_last(entry->d_name, wildcard_str) == NULL)
			continue;
		printf("%s\n", entry->d_name);
	}

	if (closedir(dir)) {
		perror("closedir");
		return NULL;
	}
	
	return (wildcard_list);
}



// int main() {
//     DIR *dir;
//     struct dirent *entry;
//     struct stat file_stat;

//     dir = opendir(".");
//     if (dir == NULL) {
//         perror("opendir");
//         return 1;
//     }
    
//     printf("Directory contents with types:\n");
//     while ((entry = readdir(dir)) != NULL) {
//         if (stat(entry->d_name, &file_stat)) {
//             perror("stat");
//             continue;
//         }
        
//         printf("%-20s ", entry->d_name);
//         if (S_ISDIR(file_stat.st_mode)) {
//             printf("(directory)\n");
//         } else if (S_ISREG(file_stat.st_mode)) {
//             printf("(regular file)\n");
//         } else {
//             printf("(other)\n");
//         }
//     }

//     if (closedir(dir)) {
//         perror("closedir");
//         return 1;
//     }
    
//     return 0;
// }
