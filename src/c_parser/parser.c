/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:52:00 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 13:54:14 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <stdlib.h>

t_ast_node	*create_ast_node(t_ast_type type)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	if (type == AST_COMMAND)
	{
		node->data.cmd.args = NULL;
		node->data.cmd.redirs = NULL;
	}
	else if (type == AST_GROUP)
		node->data.group = NULL;
	else
	{
		node->data.op.left = NULL;
		node->data.op.right = NULL;
	}
	return (node);
}

/*
** Creates a new redirection structure
*/
t_redir	*create_redirection(t_t_type type, char *file)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = strdup(file);
	redir->next = NULL;
	return (redir);
}

/*
** Adds a string to the command arguments array
*/
static char	**add_arg(char **args, char *arg)
{
	char	**new_args;
	int		i;

	i = 0;
	if (args)
		while (args[i])
			i++;
	new_args = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	if (args)
	{
		while (args[i])
		{
			new_args[i] = args[i];
			i++;
		}
		free(args);
	}
	new_args[i] = strdup(arg);
	new_args[i + 1] = NULL;
	return (new_args);
}

/*
** Parse a simple command and its arguments
*/
t_ast_node	*parse_simple_command(t_t_node **token)
{
	t_ast_node	*node;
	t_redir		*redir;
	t_redir		*last_redir;

	node = create_ast_node(AST_COMMAND);
	if (!node)
		return (NULL);
	node->data.cmd.args = NULL;
	node->data.cmd.redirs = NULL;
	while (*token && (*token)->type != PIPE && (*token)->type != SEMICOLON
		&& (*token)->type != AND && (*token)->type != OR
		&& (*token)->type != PRNT_CLOSE)
	{
		if ((*token)->type == RDRT_IN || (*token)->type == RDRT_OUT
			|| (*token)->type == APPEND || (*token)->type == HEREDOC)
		{
			redir = parse_redirection(token);
			if (!redir)
			{
				free_ast(node);
				return (NULL);
			}
			if (!node->data.cmd.redirs)
				node->data.cmd.redirs = redir;
			else
			{
				last_redir = node->data.cmd.redirs;
				while (last_redir->next)
					last_redir = last_redir->next;
				last_redir->next = redir;
			}
		}
		else if ((*token)->type == WORD)
		{
			node->data.cmd.args = add_arg(node->data.cmd.args, (*token)->value);
			*token = (*token)->next;
		}
		else
			*token = (*token)->next;
	}
	return (node);
}

/*
** Parse a redirection (< > >> <<)
*/
t_redir	*parse_redirection(t_t_node **token)
{
	t_t_type	type;
	t_redir		*redir;
	t_t_node	*file_token;

	type = (*token)->type;
	*token = (*token)->next;
	if (!(*token) || (*token)->type != WORD)
		return (NULL);
	file_token = *token;
	*token = (*token)->next;
	redir = create_redirection(type, file_token->value);
	return (redir);
}

/*
** Parse a pipeline (cmd1 | cmd2 | cmd3 ...)
*/
t_ast_node	*parse_pipeline(t_t_node **token)
{
	t_ast_node	*node;
	t_ast_node	*left;

	left = parse_simple_command(token);
	if (!left)
		return (NULL);
	if (!(*token) || (*token)->type != PIPE)
		return (left);
	node = create_ast_node(AST_PIPE);
	if (!node)
	{
		free_ast(left);
		return (NULL);
	}
	node->data.op.left = left;
	*token = (*token)->next;
	node->data.op.right = parse_pipeline(token);
	if (!node->data.op.right)
	{
		free_ast(node);
		return (NULL);
	}
	return (node);
}

/*
** Parse an AND/OR expression (cmd1 && cmd2, cmd1 || cmd2)
*/
t_ast_node	*parse_and_or(t_t_node **token)
{
	t_ast_node	*node;
	t_ast_node	*left;
	t_t_type	op_type;

	left = parse_pipeline(token);
	if (!left)
		return (NULL);
	if (!(*token) || ((*token)->type != AND && (*token)->type != OR))
		return (left);
	op_type = (*token)->type;
	node = create_ast_node(op_type == AND ? AST_AND : AST_OR);
	if (!node)
	{
		free_ast(left);
		return (NULL);
	}
	node->data.op.left = left;
	*token = (*token)->next;
	node->data.op.right = parse_and_or(token);
	if (!node->data.op.right)
	{
		free_ast(node);
		return (NULL);
	}
	return (node);
}

/*
** Parse a group expression (parentheses)
*/
t_ast_node	*parse_group(t_t_node **token)
{
	t_ast_node	*node;
	t_ast_node	*group_content;

	if (!(*token) || (*token)->type != PRNT_OPEN)
		return (parse_and_or(token));
	*token = (*token)->next;
	group_content = parse_command(token);
	if (!group_content)
		return (NULL);
	if (!(*token) || (*token)->type != PRNT_CLOSE)
	{
		free_ast(group_content);
		return (NULL);
	}
	*token = (*token)->next;
	node = create_ast_node(AST_GROUP);
	if (!node)
	{
		free_ast(group_content);
		return (NULL);
	}
	node->data.group = group_content;
	return (node);
}

/*
** Parse a command (could be a pipeline, AND/OR, or group)
*/
t_ast_node	*parse_command(t_t_node **token)
{
	return (parse_and_or(token));
}

/*
** Main parsing function - entry point
*/
t_ast_node	*parse(t_t_list *tokens)
{
	t_t_node	*token;
	t_ast_node	*ast;

	if (!tokens || !tokens->head)
		return (NULL);
	token = tokens->head;
	ast = parse_command(&token);
	return (ast);
}

/*
** Free the AST structure recursively
*/
void	free_ast(t_ast_node *node)
{
	int	i;

	if (!node)
		return;
	if (node->type == AST_COMMAND)
	{
		if (node->data.cmd.args)
		{
			i = 0;
			while (node->data.cmd.args[i])
			{
				free(node->data.cmd.args[i]);
				i++;
			}
			free(node->data.cmd.args);
		}
		while (node->data.cmd.redirs)
		{
			t_redir	*tmp = node->data.cmd.redirs;
			node->data.cmd.redirs = node->data.cmd.redirs->next;
			free(tmp->file);
			free(tmp);
		}
	}
	else if (node->type == AST_GROUP)
	{
		free_ast(node->data.group);
	}
	else
	{
		free_ast(node->data.op.left);
		free_ast(node->data.op.right);
	}
	free(node);
}

/*
** Print the AST for debugging purposes
*/
void	print_ast(t_ast_node *node, int depth)
{
	int	i;

	if (!node)
		return;
	for (i = 0; i < depth; i++)
		printf("  ");
	if (node->type == AST_COMMAND)
	{
		printf("COMMAND: ");
		if (node->data.cmd.args)
		{
			i = 0;
			while (node->data.cmd.args[i])
				printf("%s ", node->data.cmd.args[i++]);
		}
		printf("\n");
		t_redir *redir = node->data.cmd.redirs;
		while (redir)
		{
			for (i = 0; i < depth + 1; i++)
				printf("  ");
			printf("REDIR: ");
			if (redir->type == RDRT_IN)
				printf("< ");
			else if (redir->type == RDRT_OUT)
				printf("> ");
			else if (redir->type == APPEND)
				printf(">> ");
			else if (redir->type == HEREDOC)
				printf("<< ");
			printf("%s\n", redir->file);
			redir = redir->next;
		}
	}
	else if (node->type == AST_PIPE)
	{
		printf("PIPE\n");
		print_ast(node->data.op.left, depth + 1);
		print_ast(node->data.op.right, depth + 1);
	}
	else if (node->type == AST_AND)
	{
		printf("AND\n");
		print_ast(node->data.op.left, depth + 1);
		print_ast(node->data.op.right, depth + 1);
	}
	else if (node->type == AST_OR)
	{
		printf("OR\n");
		print_ast(node->data.op.left, depth + 1);
		print_ast(node->data.op.right, depth + 1);
	}
	else if (node->type == AST_GROUP)
	{
		printf("GROUP\n");
		print_ast(node->data.group, depth + 1);
	}
}

