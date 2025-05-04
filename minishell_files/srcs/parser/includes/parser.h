#include "../../../includes/minishell.h"

int			extract_non_wildcard_text(char *line, int i, char **dest);
void		extract_wildcard_content(t_wildcard_type_string *wildcard_type_string, int *char_ctr, char *line);
int			copy_wildcard_string(char **dest, char *line);
t_token_lst	*handle_wildcard_variants(t_wildcard_type_string *wildcard_string, t_token_lst *wildcard_list, t_read_dir *read_dir);
t_token_lst	*join_wildcar_token(t_read_dir *read_dir, t_wildcard_type_string *wildcard_string, char *(ft_strstr_func)(const char *str, const char *wildcard));
void		add_matching_wildcard_token(char *(ft_strstr_func)(const char *str, const char *wildcard), t_read_dir *read_dir, t_wildcard_type_string *wildcard_string, t_token_lst **wildcard_list);
int			is_word_or_quote(t_token_type type);
char		*dollar_execute(char *name, t_shell shell);
char		*name_finder(char *value, int pos);
