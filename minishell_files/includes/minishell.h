/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:32:36 by opopov            #+#    #+#             */
/*   Updated: 2025/05/07 19:28:48 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/ioctl.h>
# include <termios.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <termios.h>
# include <limits.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/stat.h>
# include <termios.h>
# include "../../libft/libft.h"

typedef struct s_shell
{
	char	**env;
	int		exp_pipe[2];
	int		last_status;
	int		heredoc_ctr;
}	t_shell;

typedef enum s_token_type
{
	TOKEN_WORD, // hello
	TOKEN_S_QUOTE, // hello"$USER"  hello    $USER D
	TOKEN_D_QUOTE, //
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

typedef enum e_note_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

typedef enum e_wildcard_type
{
	WILDCARD_START,
	WILDCARD_END,
	WILDCARD_ONLY,
	WILDCARD_NONE
}	t_wildcard_type;

typedef struct s_wildcard_type_string
{
	t_wildcard_type	type;
	char			*data;
}					t_wildcard_type_string;

typedef struct s_read_dir
{
	DIR				*dir;
	struct dirent	*entry;
	struct stat		file_stat;
}					t_read_dir;

typedef struct s_redir_lst
{
	t_token_type		type;
	char				*target;
	struct s_redir_lst	*next;
}						t_redir_lst;

typedef struct s_ast_node
{
	t_node_type	type;
	union
	{
		struct
		{
			char		*executable;
			char		**exec_argv;
			t_redir_lst	*redirs;
		}				s_cmd;
		struct
		{
			struct s_ast_node	*left;
			struct s_ast_node	*right;
		}						s_binary_op;
		struct
		{
			struct s_ast_node	*subshell;
			t_redir_lst			*sub_shell_redir;
		}						s_sub_shell;
	}	u_data;
}	t_ast_node;

t_token_lst		*token_new_node(t_token_type type, char *value);
void			token_free_list(t_token_lst *head);
t_token_lst		*token_get_last_node(t_token_lst *head);
void			token_add_node_back(t_token_lst **head, t_token_lst *new_node);
int				token_lst_size(t_token_lst *head);
int				ft_append_char(char *str, char c);
int				initialize_tokenize_struct(t_tokenize_struct *vars, char *line);
t_token_lst		*ft_tokenize(char *line, t_shell *shell);
int				is_not_special_char(char c);
int				ft_isblank(int c);
int				handle_unmatched_quotes(
					t_tokenize_struct *vars, t_token_lst **token_lst);
int				ft_setenv(char *name,
					char *value, int overwrite, t_shell *shell);
char			*ft_getenv(const char *name, t_shell shell);
void			ft_echo(char **argv);
void			ft_pwd(void);
void			ft_env(t_shell *shell);
int				ft_cd(char **argv, t_shell *shell);
int				ft_export(char **argv, t_shell *shell);
int				ft_unset(char **argv, t_shell *shell);
t_token_lst		*handle_wildcard_2(char *line);
t_ast_node		*create_cmd_node(t_node_type type,
					char *executable, char **exec_argv, t_redir_lst *redirs);
t_ast_node		*create_binary_op_node(t_node_type type,
					t_ast_node *left, t_ast_node *right);
t_ast_node		*create_subshell_node(t_node_type type,
					t_ast_node *subshell, t_redir_lst *sub_shell_redir);
void			free_ast_node(t_ast_node **node);
int				is_cmd_valid(t_token_type type);
int				is_token_valid_for_cmd(t_token_type type);
t_token_lst		*append_redirections(t_redir_lst **node_redirs,
					t_token_lst *token_lst, t_shell *shell);
int				is_quote_or_word(t_token_type type);
int				is_redirection(t_token_type type);
t_token_lst		*parse_word(t_token_lst *token_lst,
					t_ast_node **ast_node, t_shell *shell);
t_token_lst		*parse_or(t_token_lst *token_lst,
					t_ast_node **ast_node, t_shell *shell);
t_redir_lst		*new_redir_node(t_token_type type, char *target);
void			add_redir_back(t_redir_lst **lst, t_redir_lst *new_node);
void			free_redir_list(t_redir_lst **lst);
char			*return_executable_path(const char *name, t_shell *shell);
t_token_lst		*parse_cmd(t_token_lst *token_lst,
					t_ast_node **ast_node, t_shell *shell);
t_token_lst		*wildcard_function(char *line, int *char_ctr);
int				execute_cmd(t_ast_node *ast_node,
					int in_fd, int out_fd, t_shell *shell);
int				ex(t_ast_node *ast_head,
					int in_fd, int out_fd, t_shell *shell);
int				ms_strcmp_until(char *s1, char *s2, char c);
int				builtin_check(char *cmd);
int				execute_builtin(char **argv, t_shell *shell);
int				handle_heredoc(char *end_delimitor, int in_fd, char **tmp_file);
int				preprocess_heredocs(t_ast_node *node, t_shell *shell);
void			cleanup_heredocs(t_ast_node *node, t_shell *shell);
int				handle_redirection_fd(t_redir_lst *redir_lst, int *in_fd);
char			*arg_return(char *value, t_token_type type, t_shell *shell);
char			**copy_token_till_delimeter_dest(
					char **dest, int *i, int *j, char *src);
int				copy_token_till_delimeter_step2(
					char *src, int i, char *temp, t_token_lst **token_lst);
char			*copy_token_till_delimeter_step1(
					char **dest, char delimiter, int *j);
t_token_type	return_token_type(char *str);
int				send_paren_to_token_lst_paren(
					char *str, char *temp, t_tokenize_struct *vars);
int				ft_tokenize_loop_part1(
					char *line, int *i, t_tokenize_struct *vars,
					t_token_lst **token_lst);
int				is_not_special_char(char c);
t_token_type	return_token_type(char *str);
int				send_str_to_token_lst(
					char *str, t_token_lst **token_lst,
					t_token_type token_type);
int				copy_until_special_char(
					char *current_token, char *line,
					char *char_set);
int				send_paren_to_token_lst(
					char *str, t_token_lst **token_lst,
					t_token_type token_type,
					t_tokenize_struct *vars);
int				create_word_token(
					char *current_token, char *line,
					t_token_lst **token_lst, t_shell *shell);
int				is_wildcard_present(char *line);
int				handle_quotes(
					t_tokenize_struct *vars, char *line,
					t_token_lst **token_lst);
int				handle_other_tokens(char *line,
					t_token_lst **token_lst,
					t_tokenize_struct *vars);
int				append_to_token(t_token_type type,
					char *current_token, t_token_lst **token_lst);
int				handle_env_var(char *current_token,
					char *line, t_token_lst **token_lst);
int				handle_wildcard(char *current_token,
					char *line, t_token_lst **token_lst);
int				copy_token_till_delimiter(char **dest,
					char *src, char delimiter,
					t_token_lst **token_lst);
int				ft_tokenize_loop_part2_error_handler(int temp,
					t_tokenize_struct *vars,
					t_token_lst **token_lst, int *i);
void			append_to_final_word(char **final_word,
					char *temp_str, char *current_token);
int				append_final_word_to_token_lst(t_token_lst **token_lst,
					char *final_word, t_token_type type);
void			wildards_to_token_lst(char *final_word,
					t_token_lst **token_lst);
int				copy_till_delimiter_2(char *current_token,
					char *line, char delimiter);
int				send_paren_to_token_lst_paren(char *str,
					char *temp, t_tokenize_struct *vars);
int				ft_tokenize_loop_part2_error_handler(int temp,
					t_tokenize_struct *vars, t_token_lst **token_lst, int *i);
int				copy_quotes_if(char *current_token, char *line,
					char **final_word, t_shell *shell);
void			process_and_append_to_final_word(char *current_token,
					char **final_word, t_shell *shell);
int				handle_double_quote(char *current_token,
					char *line, char **final_word, t_shell *shell);
int				handle_single_quote(char *current_token,
					char *line, char **final_word);
int				ft_tokenize_loop_part2_error_handler(int temp,
					t_tokenize_struct *vars, t_token_lst **token_lst, int *i);
char			**copy_env(char **envp);
int				search_shlvl(char **env);
void			signal_handler(int signum);
void			env_free(char **str);
int				is_first_token_valid(t_token_type type);
char			*arg_word_return(char *value, t_shell shell);

static volatile int	g_signal_received;

#endif
