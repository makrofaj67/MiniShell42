/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_parser.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:05:05 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 13:54:14 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef C_PARSER_H
# define C_PARSER_H

# include "b_lexer.h"

/* AST Node Types */
typedef enum e_ast_type
{
	AST_COMMAND,      /* Simple command */
	AST_PIPE,         /* Pipe operator | */
	AST_REDIR_IN,     /* Redirection < */
	AST_REDIR_OUT,    /* Redirection > */
	AST_APPEND,       /* Append >> */
	AST_HEREDOC,      /* Here document << */
	AST_AND,          /* Logical AND && */
	AST_OR,           /* Logical OR || */
	AST_GROUP         /* Grouping with parentheses () */
}	t_ast_type;

/* Redirection structure */
typedef struct s_redir
{
	t_t_type        type;    /* Redirection type from lexer */
	char            *file;   /* Target file name */
	struct s_redir  *next;   /* Next redirection */
}	t_redir;

/* Command structure */
typedef struct s_cmd
{
	char    **args;          /* Command arguments (args[0] is the command) */
	t_redir *redirs;         /* List of redirections */
}	t_cmd;

/* AST Node structure */
typedef struct s_ast_node
{
	t_ast_type          type;      /* Node type */
	union {
		t_cmd               cmd;    /* For AST_COMMAND */
		struct {
			struct s_ast_node  *left;  /* Left child */
			struct s_ast_node  *right; /* Right child */
		}                   op;     /* For operators like PIPE, AND, OR */
		struct s_ast_node  *group;  /* For parenthesized group */
	}                   data;
}	t_ast_node;

/* Main parser function */
t_ast_node	*parse(t_t_list *tokens);

/* Parser helper functions */
t_ast_node	*parse_command(t_t_node **token);
t_ast_node	*parse_pipeline(t_t_node **token);
t_ast_node	*parse_and_or(t_t_node **token);
t_ast_node	*parse_group(t_t_node **token);
t_ast_node	*parse_simple_command(t_t_node **token);
t_redir		*parse_redirection(t_t_node **token);

/* AST Node management functions */
t_ast_node	*create_ast_node(t_ast_type type);
void		free_ast(t_ast_node *node);
void		print_ast(t_ast_node *node, int depth); /* For debugging */

#endif
