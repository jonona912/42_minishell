/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:57:46 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/11 13:05:48 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H

#include "minishell.h"

typedef enum {
	NODE_CMD,
	NODE_REDIRECTION_IN,
	NODE_REDIRECTION_OUT,
	NODE_APPEND,
	NODE_HEREDOC
} t_node_type;

typedef struct s_ast_node {
	t_node_type type; // redirection, cmds, 
	union {
		struct {
			char *executable; // for cmds cat, ....
			char **exec_argv;
		} cmd;
		struct {
			t_token_type op; // pipe , OR, AND
			struct s_ast_node* left;
			struct s_ast_node* right;
		} binary_op;
	} data;
}	t_ast_node;




#endif