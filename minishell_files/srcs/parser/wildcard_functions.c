#include "../../includes/minishell.h"

// int is_wildcard;
// int 
// ******
// ****asdf
// asdf*****

t_wildcard_type	find_wildcard_type(char *line)
{
	t_wildcard_type	wildcard_type;
	int	i;

	i = 0;
	wildcard_type = WILDCARD_NONE;
	while (is_not_special_char(line[i]) && line[i] == '*')
	{
		wildcard_type = WILDCARD_ONLY;
		i++;
	}
	while (is_not_special_char(line[i]) && line[i] != '*')
	{
		if (wildcard_type == WILDCARD_ONLY)
			wildcard_type = WILDCARD_START;
		i++;
	}
	while (is_not_special_char(line[i]) && line[i] == '*')
	{
		if (wildcard_type == WILDCARD_NONE)
			wildcard_type = WILDCARD_END;;;
		i++;
	}
	return (wildcard_type);
}

int	copy_wildcard_string(char **dest, char *line)
{
	int		i;
	char	*traverse;
	int		len;

	len = 0;
	i = 0;
	while (is_not_special_char(line[i]) && line[i] == '*')
		i++;
	traverse = (line + i);
	while (is_not_special_char(*traverse) && *traverse != '*')
	{
		len++;
		traverse++;
	}
	*dest = (char *)malloc(len + 1);
	if (!*dest)
	{
		perror("malloc");
		return (-1); // this won't work properly
	}
	len = 0;
	while (is_not_special_char(line[i]) && line[i] != '*')
	{
		(*dest)[len] = line[i];
		i++;
		len++;
	}
	while (line[i] && line[i] == '*')
		i++;
	(*dest)[len] = '\0';
	return (i);
}

t_wildcard_type_string	*identify_wildcard_type(char *line, int *char_ctr)
{
	t_wildcard_type_string	*wildcard_type_string;

	wildcard_type_string = (t_wildcard_type_string *)malloc(sizeof(t_wildcard_type_string));
	if (!wildcard_type_string)
	{
		perror("malloc");
		return NULL; // this won't work properly
	}
	if (!line || line[0] == '\0')
	{
		wildcard_type_string->type = WILDCARD_NONE;
		return wildcard_type_string;
	}
	wildcard_type_string->type = find_wildcard_type(line);
	if (wildcard_type_string->type == WILDCARD_START || wildcard_type_string->type == WILDCARD_END)
	{
		*char_ctr = copy_wildcard_string(&wildcard_type_string->data, line);
	}
	else if (wildcard_type_string->type == WILDCARD_ONLY)
	{
		while (is_not_special_char(line[*char_ctr]) && line[*char_ctr] == '*')
			(*char_ctr)++;
		wildcard_type_string->data = NULL;
	}
	else
	{
		wildcard_type_string->data = NULL;
	}
	return (wildcard_type_string);
}

t_token_lst	*join_wildcar_token(t_read_dir *read_dir, t_wildcard_type_string *wildcard_string, char *(ft_strstr_func)(const char *str, const char *wildcard))
{
	t_token_lst	*wildcard_list;
	char		*str;

	wildcard_list = NULL;
	while ((read_dir->entry = readdir(read_dir->dir)) != NULL)
	{
		if (stat(read_dir->entry->d_name, &read_dir->file_stat))
		{
			perror("stat");
			continue;
		}
		if (read_dir->entry->d_name[0] == '.')
				continue ;
		if (ft_strstr_func)
		{
			str = ft_strstr_func(read_dir->entry->d_name, wildcard_string->data);
			if (str)
			{
				str = ft_strdup(read_dir->entry->d_name);
				token_add_node_back(&wildcard_list, token_new_node(TOKEN_WORD, str));
			}
		}
		else
		{
			str = ft_strdup(read_dir->entry->d_name);
			token_add_node_back(&wildcard_list, token_new_node(TOKEN_WORD, str));
		}
	}
	return wildcard_list;
}

t_token_lst	*wildcard_function(char *line, int *char_ctr)
{
	t_read_dir				read_dir;
	t_token_lst				*wildcard_list;
	t_wildcard_type_string	*wildcard_string;

	wildcard_string = identify_wildcard_type(line, char_ctr);
	wildcard_list = NULL;
	read_dir.dir = opendir(".");
	if (read_dir.dir == NULL)
	{
		perror("opendir");
		return NULL;
	}
	if (wildcard_string->type == WILDCARD_START)
		wildcard_list = join_wildcar_token(&read_dir, wildcard_string, ft_strstr_last);
	else if (wildcard_string->type == WILDCARD_END)
		wildcard_list = join_wildcar_token(&read_dir, wildcard_string, ft_strstr_first);
	else if (wildcard_string->type == WILDCARD_ONLY)
		wildcard_list = join_wildcar_token(&read_dir, wildcard_string, NULL);
	if (closedir(read_dir.dir)) {
		perror("closedir");
		return NULL;
	}
	if (wildcard_string->data)
	{
		free(wildcard_string->data);
	}
	free(wildcard_string);
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
