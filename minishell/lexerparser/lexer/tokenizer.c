#include "linked_list_operations.c"

char	*extract_token(char *command, int start, int end)
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

/**
 * Tüm token listesini temizleme
 */
void	free_token_list(t_token_list *list)
{
	t_token_node	*current;
	t_token_node	*next;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	free(list);
}

/**
 * Özel karakter kontrolü
 */
int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/**
 * Boşlukları atlama
 */
void	skip_spaces(char *command, int *i)
{
	while (command[*i] && isspace(command[*i]))
		(*i)++;
}

/**
 * Tırnak işleme
 */
int	handle_quotes(char *command, int start, int *has_error)
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
	*has_error = 1;
	return (-1);
}

/**
 * Token tipini belirleme
 */
t_token_type	determine_token_type(char *command, int start, int end)
{
	if (command[start] == '"' || command[start] == '\'')
		return (WORD);
	if (end - start == 1)
	{
		if (command[start] == '|')
			return (PIPE);
		else if (command[start] == '<')
			return (REDIR_IN);
		else if (command[start] == '>')
			return (REDIR_OUT);
	}
	else if (end - start == 2)
	{
		if (command[start] == '<' && command[start + 1] == '<')
			return (HEREDOC);
		else if (command[start] == '>' && command[start + 1] == '>')
			return (APPEND);
	}
	return (WORD);
}

/**
 * Token bitiş indeksini bulma
 */
int	get_token_end(char *command, int start, int *has_error)
{
	int	i;

	*has_error = 0;
	i = start;
	if (command[i] == '"' || command[i] == '\'')
		return (handle_quotes(command, i, has_error));
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

/**
 * Token değerini oluşturma
 */
char	*get_token_value(char *cmd, int start, int end, int is_quoted)
{
	if (is_quoted)
		return (extract_token(cmd, start + 1, end - 1));
	return (extract_token(cmd, start, end));
}

/**
 * Hata mesajı oluşturma
 */
char	*create_error_message(char *error_msg, char *error_token)
{
	char	*value;
	char	*full_msg;
	int		len;

	if (!error_token)
		return (strdup(error_msg));
	len = strlen(error_msg) + strlen(error_token) + 3;
	full_msg = malloc(len);
	if (!full_msg)
		return (NULL);
	snprintf(full_msg, len, "%s: %s", error_msg, error_token);
	value = full_msg;
	return (value);
}

/**
 * Hata token'ı oluşturma
 */
int	add_error_token(t_token_list *list, char *error_msg, char *error_token)
{
	char			*value;
	t_token_node	*node;

	value = create_error_message(error_msg, error_token);
	if (!value)
		return (0);
	node = create_token_node(ERROR, value);
	if (!node)
	{
		free(value);
		return (0);
	}
	add_token(node, list);
	return (1);
}

/**
 * Token oluşturma ve ekleme
 */
int	add_token_to_list(char *cmd, int start, int end, t_token_list *list)
{
	t_token_type	type;
	char			*value;
	t_token_node	*node;
	int				is_quoted;

	is_quoted = (cmd[start] == '"' || cmd[start] == '\'');
	type = determine_token_type(cmd, start, end);
	value = get_token_value(cmd, start, end, is_quoted);
	if (!value)
		return (0);
	node = create_token_node(type, value);
	if (!node)
	{
		free(value);
		return (0);
	}
	add_token(node, list);
	return (1);
}

/**
 * Bir token işleme
 */
int	process_token(char *cmd, int *i, t_token_list *list)
{
	int	start;
	int	end;
	int	has_error;

	skip_spaces(cmd, i);
	if (!cmd[*i])
		return (1);
	start = *i;
	end = get_token_end(cmd, start, &has_error);
	if (has_error || end == -1)
	{
		add_error_token(list, "Unclosed quote error", 
			extract_token(cmd, start, start + 10));
		return (0);
	}
	if (!add_token_to_list(cmd, start, end, list))
	{
		free_token_list(list);
		return (-1);
	}
	*i = end;
	return (1);
}

/**
 * Command string'inden token listesi oluşturma
 */
t_token_list	*tokenize_command(char *command)
{
	t_token_list	*list;
	int				i;
	int				result;

	list = init_token_list();
	if (!list)
		return (NULL);
	i = 0;
	while (command[i])
	{
		result = process_token(command, &i, list);
		if (result == 0)
			return (list);
		if (result == -1)
			return (NULL);
	}
	return (list);
}

/**
 * Token tipini string'e dönüştürme
 */
const char	*token_type_to_string(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == REDIR_IN)
		return ("REDIR_IN");
	else if (type == REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == APPEND)
		return ("APPEND");
	else if (type == HEREDOC)
		return ("HEREDOC");
	else if (type == ERROR)
		return ("ERROR");
	else
		return ("UNKNOWN");
}


