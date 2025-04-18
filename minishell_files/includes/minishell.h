#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <termios.h>
#include <limits.h>
# include <fcntl.h> // for open
#include <dirent.h>
#include <sys/stat.h>


#include "../../libft/libft.h"

// # include <sys/types.h>

typedef struct s_shell
{
	char	**env;
	char	*cd;
	char	*old_cd;
	int		exp_pipe[2];
	int		last_status;
}	t_shell;


typedef enum	s_token_type
{
	TOKEN_WORD,
	TOKEN_S_QUOTE,
	TOKEN_D_QUOTE,
	TOKEN_WILDCARD, // not handled
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



// // make separate lists for tokens and commands
// typedef struct s_cmd_lst
// {
// 	char			*path;
// 	char			**av;
// 	struct s_cmd_lst	*next;
// }	t_cmd_lst;

// typedef struct	s_command
// {
// 	char **tokens;
// 	char *current_cmd_lst;
// 	char *input_file;
// 	char *output_file;
// 	struct s_command *next;
// 	int	quote;
// 	int	redirection;
// 	int	pipe;
// }	t_command;

typedef enum {
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	// NODE_GROUP
} t_node_type;


// Structure for a single redirection or heredoc
typedef struct s_redir_lst {
    t_token_type type;      // REDIR_INPUT, REDIR_OUTPUT, REDIR_APPEND, REDIR_HEREDOC
    char *target;           // File path (e.g., "input.txt") or heredoc delimiter (e.g., "EOF")
    struct s_redir_lst *next; // Linked list for multiple redirections
} t_redir_lst;

// AST node structure
typedef struct s_ast_node
{
	t_node_type type; // CMD or PIPE
	t_redir_lst	*paren_redirections; // if type == SUBSHELL_NODE ()
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
	} data;
}	t_ast_node;

// typedef struct s_pid_lst
// {
// 	pid_t				pid;
// 	struct s_pid_lst	*next;
// }	t_pid_lst;

typedef enum
{
	WILDCARD_START,
	WILDCARD_END,
	WILDCARD_ONLY,
	WILDCARD_NONE
} t_wildcard_type;

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


///////////// lists /////////////
// ms_command_lst.c
// t_cmd_lst	*ms_new_node(char *value, char **argv);
// void		ms_free_list(t_cmd_lst *head);
// t_cmd_lst	*ms_get_last_node(t_cmd_lst *head);
// void		ms_add_node_back(t_cmd_lst **head, t_cmd_lst *new_node);

// ms_token_lst.c
t_token_lst	*token_new_node(t_token_type type, char *value);
void		token_free_list(t_token_lst *head);
t_token_lst	*token_get_last_node(t_token_lst *head);
void		token_add_node_back(t_token_lst **head, t_token_lst *new_node);

////////////// tokenizer ////////////
// ms_tokenizer.c
int			ft_append_char(char *str, char c);
void		initialize_tokenize_struct(t_tokenize_struct *vars, char *line);
t_token_lst	*ft_tokenize(char *line);
int	is_not_special_char(char c);

int		ft_isblank(int c);
int		handle_unmatched_quotes(t_tokenize_struct *vars, t_token_lst **token_lst);
int		process_redirection(t_tokenize_struct *vars, t_token_lst **token_lst, char *line, int *i, t_token_type token_type, int step);
int		ft_setenv(char *name, char *value, int overwrite, t_shell *shell);
char	*ft_getenv(const char *name, t_shell shell);
void	ft_echo(char **argv);
void	ft_pwd();
int		ft_exit(char **argv);
void	ft_env(t_shell *shell);
// int		ft_cd (char **argv, t_shell *shell);
int		ft_export(char **argv, t_shell *shell);
int		ft_unset(char **argv, t_shell *shell);

/////////////////////// parser /////////////////////////

// ast_binary_tree_function.c
t_ast_node *create_cmd_node(t_node_type type, char *executable, char **exec_argv, t_redir_lst *redirs);
t_ast_node *create_binary_op_node(t_node_type type, t_ast_node *left, t_ast_node *right);

// parser_helper_1.c
t_token_lst	*append_redirections(t_ast_node **ast_node, t_token_lst *token_lst);
int	is_quote_or_word(t_token_type type);
int	is_redirection(t_token_type type);

// parser_parse_word.c
t_token_lst	*parse_word(t_token_lst *token_lst, t_ast_node **ast_node, t_shell shell);

// parser.c
t_token_lst	*parse_pipe(t_token_lst *token_lst, t_ast_node **ast_node, t_shell shell);
t_token_lst	*parse_and(t_token_lst *token_lst, t_ast_node **ast_node, t_shell shell);
t_token_lst	*parse_or(t_token_lst *token_lst, t_ast_node **ast_node, t_shell shell);
char		*arg_return(char *value, t_token_type type, t_shell shell);

// redirection_functions.c
t_redir_lst *new_redir_node(t_token_type type, char *target);
void add_redir_back(t_redir_lst **lst, t_redir_lst *new_node);
void free_redir_list(t_redir_lst **lst);

// return_executable_path.c
char *return_executable_path(const char *name);


// wildcard_functions.c
t_token_lst *wildcard_function(char *line, int *char_ctr);

///////////////////// execute ////////////////////////
// int execute(t_ast_node *ast_head, int pipe_direction, int pipe_fd[2]);
// int	run_pipeline(t_ast_node *ast_head, t_shell *shell);
int	execute(t_ast_node *ast_head, int in_fd, int out_fd, t_shell *shell);
// int	dup2_fd(int fd, int std_fd_fileno);
int	ms_strcmp_until(char *s1, char *s2, char c);
int	builtin_check(char *cmd);
int	execute_builtin(char **argv, t_shell *shell);


void debug_env(t_shell *shell);


#endif
