/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:23:31 by rakman            #+#    #+#             */
/*   Updated: 2025/04/26 19:02:31 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static void	skip_spaces(char *command, int *i)
{
	while (command[*i] && isspace(command[*i]))
		(*i)++;
}

static int	handle_quotes(char *command, int start)
{
	int	i;
	int	quote;

	i = start;
	quote = command[i];
	i++;
	while (command[i] && command[i] != quote)
		i++;
	if (command[i] == quote)
		return (i + 1);
	return (-1);
}

static int	get_token_end(char *command, int start)
{
	int	i;

	i = start;
	if (command[i] == '"' || command[i] == '\'')
	{
		i = handle_quotes(command, i);
		if (i == -1)
			return (-1);
		return (i);
	}
	if (is_special_char(command[i]))
	{
		if ((command[i] == '<' && command[i + 1] == '<') || 
			(command[i] == '>' && command[i + 1] == '>'))
			return (i + 2);
		return (i + 1);
	}
	while (command[i] && !isspace(command[i]) && !is_special_char(command[i]))
		i++;
	return (i);
}

static char	*extract_token(char *command, int start, int end)
{
	char	*token;
	int		i;

	token = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!token)
		return (NULL);
	i = 0;
	while (start + i < end)
	{
		token[i] = command[start + i];
		i++;
	}
	token[i] = '\0';
	return (token);
}

static char	*get_token_value(char *cmd, int start, int end)
{
	int	is_quoted;

	is_quoted = (cmd[start] == '"' || cmd[start] == '\'');
	if (is_quoted)
		return (extract_token(cmd, start + 1, end - 1));
	return (extract_token(cmd, start, end));
}



char	**init_tokens_array(int size)
{
	char	**tokens;

	tokens = (char **)malloc(sizeof(char *) * (size + 1));
	if (!tokens)
		return (NULL);
	tokens[size] = NULL;
	return (tokens);
}

t_token_type	determine_token_type(char *token)
{
	if (strcmp(token, "|") == 0)
		return (PIPE);
	else if (strcmp(token, "<") == 0)
		return (REDIR_IN);
	else if (strcmp(token, ">") == 0)
		return (REDIR_OUT);
	else if (strcmp(token, "<<") == 0)
		return (HEREDOC);
	else if (strcmp(token, ">>") == 0)
		return (APPEND);
	return (WORD);
}

t_token_node	*token_to_node(char *token)
{
	t_token_type	type;
	char			*value;

	type = determine_token_type(token);
	value = strdup(token);
	if (!value)
		return (NULL);
	return (create_token_node(type, value));
}

t_token_list	*create_token_list_from_array(char **tokens)
{
	t_token_list	*list;
	t_token_node	*node;
	int				i;

	list = init_token_list();
	if (!list)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		node = token_to_node(tokens[i]);
		if (!node)
		{
			free_token_list(list);
			return (NULL);
		}
		add_token(node, list);
		i++;
	}
	return (list);
}

void	free_tokens_array(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

static int	process_token(char *command, int *pos, char **tokens, int i)
{
	int	start;
	int	end;

	skip_spaces(command, pos);
	start = *pos;
	end = get_token_end(command, start);
	if (end == -1)
		return (-1);
	tokens[i] = get_token_value(command, start, end);
	if (!tokens[i])
		return (-1);
	*pos = end;
	return (0);
}






static int	count_tokens(char *command)
{
	int	i;
	int	count;
	int	end;

	i = 0;
	count = 0;
	while (command[i])
	{
		skip_spaces(command, &i);
		if (!command[i])
			break ;
		end = get_token_end(command, i);
		if (end == -1)
			return (-1);
		count++;
		i = end;
	}
	return (count);
}

static int	setup_tokens(char *command, char ***tokens)
{
	int	token_count;

	token_count = count_tokens(command);
	if (token_count == -1)
		return (-1);
	*tokens = init_tokens_array(token_count);
	if (!(*tokens))
		return (-1);
	return (token_count);
}

char	**extract_tokens(char *command)
{
	char	**tokens;
	int		token_count;
	int		i;
	int		pos;
	int		result;

	token_count = setup_tokens(command, &tokens);
	if (token_count == -1 || !tokens)
		return (NULL);
	i = 0;
	pos = 0;
	while (i < token_count)
	{
		result = process_token(command, &pos, tokens, i);
		if (result == -1)
		{
			free_tokens_array(tokens);
			return (NULL);
		}
		i++;
	}
	return (tokens);
}
