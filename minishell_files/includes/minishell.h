#ifndef MINISHELL_H
#define MINISHELL_H

#include <sys/ioctl.h>
#include <termios.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <termios.h>
#include <limits.h>
# include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

#include "../../libft/libft.h"

typedef struct s_shell
{
	char	**env;
	int		exp_pipe[2];
	int		last_status;
	int		heredoc_temp_counter;
}	t_shell;

typedef enum	s_token_type
{
	TOKEN_WORD,
	TOKEN_S_QUOTE,
	TOKEN_D_QUOTE,
	TOKEN_WILDCARD,
	TOKEN_PIPE,
	TOKEN_REDIRECTION_IN,
	TOKEN_REDIRECTION_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ENV_VAR,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_L_PAREN,
	TOKEN_R_PAREN,
	TOKEN_END
}	t_token_type;

typedef struct s_token_lst
{
	t_token_type		type;
	char				*value;
	struct s_token_lst	*next;
}	t_token_lst;

typedef struct s_tokenize_struct
{
	char	*current_token;
	int		is_s_quote;
	int		is_d_quote;
	int		is_parenthesis;
	int		paren_counter;
}	t_tokenize_struct;

typedef enum {
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
} t_node_type;

typedef enum
{
	WILDCARD_START,
	WILDCARD_END,
	WILDCARD_ONLY,
	WILDCARD_NONE
}	t_wildcard_type;

typedef struct s_wildcard_type_string
{
	t_wildcard_type type;
	char *data;
}	t_wildcard_type_string;

typedef struct s_read_dir
{
	DIR *dir;
	struct dirent *entry;
	struct stat file_stat;
}	t_read_dir;


typedef struct s_redir_lst {
    t_token_type type;
    char *target;
    struct s_redir_lst *next;
} t_redir_lst;

// AST node structure
typedef struct s_ast_node
{
	t_node_type type; // CMD or PIPE
	union {
		struct
		{
			char *executable;
			char **exec_argv;
			t_redir_lst *redirs;
		} cmd;
		struct
		{
			struct s_ast_node* left;
			struct s_ast_node* right;
		} binary_op;
		struct
		{
			struct s_ast_node* subshell;
			t_redir_lst *sub_shell_redir;
		} sub_shell;
	} data;
}	t_ast_node;




///////////// lists /////////////

// ms_token_lst.c
t_token_lst	*token_new_node(t_token_type type, char *value);
void		token_free_list(t_token_lst *head);
t_token_lst	*token_get_last_node(t_token_lst *head);
void		token_add_node_back(t_token_lst **head, t_token_lst *new_node);
int	token_lst_size(t_token_lst *head);
////////////// tokenizer ////////////
// ms_tokenizer.c
int			ft_append_char(char *str, char c);
int			initialize_tokenize_struct(t_tokenize_struct *vars, char *line);
t_token_lst	*ft_tokenize(char *line);
int			is_not_special_char(char c);
// NEEDS TO BE CHANGED
int		ft_isblank(int c);
int		handle_unmatched_quotes(t_tokenize_struct *vars, t_token_lst **token_lst);
int		ft_setenv(char *name, char *value, int overwrite, t_shell *shell);
char	*ft_getenv(const char *name, t_shell shell);
void	ft_echo(char **argv);
void	ft_pwd();
// int		ft_exit(char **argv);
void	ft_env(t_shell *shell, int is_exp);
int		ft_cd (char **argv, t_shell *shell);
int		ft_export(char **argv, t_shell *shell);
int		ft_unset(char **argv, t_shell *shell);

/////////////////////// parser /////////////////////////

// ast_binary_tree_function.c
t_ast_node *create_cmd_node(t_node_type type, char *executable, char **exec_argv, t_redir_lst *redirs);
t_ast_node *create_binary_op_node(t_node_type type, t_ast_node *left, t_ast_node *right);
t_ast_node	*create_subshell_node(t_node_type type, t_ast_node *subshell, t_redir_lst *sub_shell_redir);
void free_ast_node(t_ast_node *node);
// parser_helper_1.c
t_token_lst	*append_redirections(t_redir_lst **node_redirs, t_token_lst *token_lst, t_shell *shell);
int	is_quote_or_word(t_token_type type);
int	is_redirection(t_token_type type);

// parser_parse_word.c
t_token_lst	*parse_word(t_token_lst *token_lst, t_ast_node **ast_node, t_shell *shell);

// parser.c
t_token_lst	*parse_or(t_token_lst *token_lst, t_ast_node **ast_node, t_shell *shell);

// redirection_functions.c
t_redir_lst *new_redir_node(t_token_type type, char *target);
void add_redir_back(t_redir_lst **lst, t_redir_lst *new_node);
void free_redir_list(t_redir_lst **lst);

// return_executable_path.c
char	*return_executable_path(const char *name, t_shell *shell);

// parse_cmd.c
t_token_lst *parse_cmd(t_token_lst *token_lst, t_ast_node **ast_node, t_shell *shell);


// wildcard_functions.c
t_token_lst	*wildcard_function(char *line, int *char_ctr);

///////////////////// execution ////////////////////////
// execute_command.c
// int execute_cmd(t_ast_node *ast_node, int in_fd, int out_fd);
int execute_cmd(t_ast_node *ast_node, int in_fd, int out_fd, t_shell *shell);

// execution.c
// int execute(t_ast_node *ast_head, int in_fd, int out_fd);
int	execute(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell);
int	ms_strcmp_until(char *s1, char *s2, char c);
int	builtin_check(char *cmd);
int	execute_builtin(char **argv, t_shell *shell);

// here_doc.c
// int run_heredoc(char *end_delimitor, int *in_fd);
int	handle_heredoc(char *end_delimitor, int in_fd);

// here_doc_processing.c
int preprocess_heredocs(t_ast_node *node, t_shell *shell);
void	cleanup_heredocs(t_ast_node *node, t_shell *shell);

// redirections.c
int handle_redirection_fd(t_redir_lst *redir_lst, int *in_fd);


// check_user_input.c
int	check_user_input(char **line);

/////////////////////////// print_structs ///////////////////////////
void	ft_print_tokens(t_token_lst *token_lst);
void print_ast(t_ast_node *root);

// NEW FUNCTIONS FROM MERGING BUILTINS
char	*arg_return(char *value, t_token_type type, t_shell *shell);
void	set_and_move_eight_bits_left(int *x, int set_num);


// tokenizer
char			**copy_token_till_delimeter_dest(char **dest, int *i, int *j, char *src);
int				copy_token_till_delimeter_step2(char *src, int i, char *temp, t_token_lst **token_lst);
char			*copy_token_till_delimeter_step1(char **dest, char delimiter, int *j);
t_token_type	return_token_type(char *str);
int				send_paren_to_token_lst_paren(char *str, char *temp, t_tokenize_struct *vars);
int				ft_tokenize_loop_part1(char *line, int *i, t_tokenize_struct *vars, t_token_lst **token_lst);
int				ft_tokenize_loop_part2(char *line, int *i, t_tokenize_struct *vars, t_token_lst **token_lst);
int				is_not_special_char(char c);
t_token_type	return_token_type(char *str);
int				send_str_to_token_lst(char *str, t_token_lst **token_lst, t_token_type token_type);
int				copy_until_special_char(char *current_token, char *line, char *char_set);
int				send_paren_to_token_lst(char *str, t_token_lst **token_lst, t_token_type token_type, t_tokenize_struct *vars);
int				create_word_token(char *current_token, char *line, t_token_lst **token_lst);
int				is_wildcard_present(char *line);
int				handle_quotes(t_tokenize_struct *vars, char *line, t_token_lst **token_lst);
int				handle_other_tokens(char *line, t_token_lst **token_lst, t_tokenize_struct *vars);
int				append_to_token(t_token_type type, char *current_token, t_token_lst **token_lst);
int				handle_env_var(char *current_token, char *line, t_token_lst **token_lst);
int				handle_wildcard(char *current_token, char *line, t_token_lst **token_lst);
int				copy_token_till_delimiter(char **dest, char *src, char delimiter, t_token_lst **token_lst);
int				ft_tokenize_loop_part2_error_handler(int temp, t_tokenize_struct *vars, t_token_lst **token_lst, int *i);


//parser
int			copy_wildcard_string_loop(char *line, int i, char **dest);
void		identify_wildcard_type_if(t_wildcard_type_string *wildcard_type_string, int *char_ctr, char *line);
int			copy_wildcard_string(char **dest, char *line);
t_token_lst	*wildcard_function_if(t_wildcard_type_string *wildcard_string, t_token_lst *wildcard_list, t_read_dir *read_dir);
t_token_lst	*join_wildcar_token(t_read_dir *read_dir, t_wildcard_type_string *wildcard_string, char *(ft_strstr_func)(const char *str, const char *wildcard));
void		join_wildcar_token_if(char *(ft_strstr_func)(const char *str, const char *wildcard), t_read_dir *read_dir, t_wildcard_type_string *wildcard_string, t_token_lst **wildcard_list);
int			is_word_or_quote(t_token_type type);
t_token_lst	*populate_command_data_loop2(t_token_lst *token_lst, t_shell *shell, t_ast_node *ast_node);
int			populate_command_data_loop1(t_token_lst *token_lst);
char		*dollar_execute(char *name, t_shell shell);
char		*name_finder(char *value, int pos);


//execution
void	handle_subshell_if(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell);
void	execute_cmd_parent_free(t_shell *shell, char **new_env);
int		execute_cmd_parent_loop_if(char **new_env, int *i, int *pipe_fd, int *len);
int		execute_cmd_parent_loop(int count, char **new_env, int *pipe_fd);
void	execute_cmd_child_fd(int in_fd, int out_fd);
void	execute_cmd_child_builtin_loop(int count, int *pipe_fd, t_shell *shell);
int		get_exit_status(int status);
void	execute_cmd_child_beginning(t_ast_node *ast_node, int *in_fd);
void	execute_cmd_child_if_else(t_ast_node *ast_node, t_shell *shell);
void	execute_cmd_child_fd(int in_fd, int out_fd);
void	close_pipe_fd(int *pipe_fd);
void	handle_pipe_left_pid_error(int *pipe_fd);
void	handle_pipe_left_pid_child(int *pipe_fd, t_ast_node *ast_head, int in_fd, t_shell *shell);


//built_in_commands
int		ft_cd_special_cases(char **argv, t_shell *shell, char **tmp, char *oldpwd);
void	ft_echo_loop(char **argv, int *i);
int		ft_exit(char **argv);
int		is_n_flag(char *argv);


// main


static volatile int  g_signal_received;


#endif
