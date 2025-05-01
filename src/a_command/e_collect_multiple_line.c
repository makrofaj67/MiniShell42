/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_collect_multiple_line.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:54:26 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 20:16:06 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/* Reference the global signal variable */
extern volatile sig_atomic_t g_signal_received;

/* Checks quote context for a position in a string */
static int	check_quotes(char *str, int pos)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (i < pos && str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}

/* Trims trailing spaces outside quotes */
static char	*trim_trailing_spaces(char *str)
{
	int	len;
	int	i;

	len = strlen(str);
	i = len - 1;
	while (i >= 0 && (str[i] == ' ' || str[i] == '\t'))
		i--;
	if (i == len - 1)
		return (str);
	if (!check_quotes(str, i + 1))
		str[i + 1] = '\0';
	return (str);
}

/* Identifies continuation character at end of line */
static int	get_continuation_char(const char *str)
{
	int	len;
	int	i;

	len = strlen(str);
	if (len == 0)
		return (0);
	i = len - 1;
	while (i >= 0 && (str[i] == ' ' || str[i] == '\t'))
		i--;
	if (i >= 0 && (str[i] == '\\' || str[i] == '|' || str[i] == '('))
		return (str[i]);
	return (0);
}

/* Checks if line starts with quote */
static int	is_quote_start(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	return (str[i] == '"' || str[i] == '\'');
}

/* Removes trailing backslash */
static char	*remove_trailing_backslash(char *cmd)
{
	int	len;

	len = strlen(cmd);
	if (len > 0 && cmd[len - 1] == '\\')
		cmd[len - 1] = '\0';
	return (cmd);
}

/* Checks if command is syntactically complete */
static int	is_command_complete(char *command)
{
	int	paren_status;

	paren_status = parentheses_status(command);
	if (paren_status < 0)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token ')'\n");
		return (1);
	}
	if (!(paren_status > 0 || has_unclosed_quotes(command)
			|| has_end_backslash(command)))
		return (1);
	return (0);
}

/* Handle backslash continuation */
static char	*process_backslash_cont(char *cmd, char *next_line)
{
	char	*result;

	trim_trailing_spaces(cmd);
	remove_trailing_backslash(cmd);
	result = ft_strjoin(cmd, next_line);
	free(next_line);
	free(cmd);
	return (result ? result : cmd);
}

/* Handle pipe and parenthesis continuation */
static char	*handle_pipe_paren(char *cmd, char *next_line)
{
	char	*result;
	char	*temp;

	trim_trailing_spaces(cmd);
	if (next_line[0] != ' ' && next_line[0] != '\t')
	{
		temp = ft_strjoin(cmd, " ");
		if (!temp)
		{
			free(next_line);
			return (cmd);
		}
		result = ft_strjoin(temp, next_line);
		free(temp);
	}
	else
		result = ft_strjoin(cmd, next_line);
	free(next_line);
	free(cmd);
	return (result ? result : cmd);
}

/* Handle quoted string continuation */
static char	*handle_quote(char *cmd, char *next_line)
{
	char	*result;
	char	*temp;

	// Tırnak içerisindeki satır sonlarını korumalıyız, \n ekleyelim
	temp = ft_strjoin(cmd, "\n");
	if (!temp)
	{
		free(next_line);
		return (cmd);
	}
	result = ft_strjoin(temp, next_line);
	free(temp);
	free(next_line);
	free(cmd);
	return (result ? result : cmd);
}

/* Combines multi-line input according to shell rules */
static char	*combine_cmd_with_newline(char *cmd, char *next_line)
{
	char	*result;
	int		cont_char;
	
	if (!cmd || !next_line)
		return (cmd);
	
	// Tırnak içerisinde olup olmadığımızı kontrol et
	// Eğer cmd'nin son karakteri tek veya çift tırnak ise veya
	// tırnak içerisinde kalıyorsak \n eklemeliyiz
	if (check_quotes(cmd, strlen(cmd)))
	{
		// Tırnak içerisindeyiz, newline eklenmeli
		return (handle_quote(cmd, next_line));
	}
	
	cont_char = get_continuation_char(cmd);
	if (cont_char == '\\')
		return (process_backslash_cont(cmd, next_line));
	else if (cont_char == '|' || cont_char == '(')
		return (handle_pipe_paren(cmd, next_line));
	else if (is_quote_start(next_line))
		return (handle_quote(cmd, next_line));
	
	result = ft_strjoin(cmd, next_line);
	free(next_line);
	free(cmd);
	return (result ? result : cmd);
}

/* Collects multi-line input until the command is complete */
char	*collect_multiple_line(char *command, char *prompt, int backslash)
{
	char	*next_line;

	(void)prompt;
	(void)backslash;
	while (1)
	{
		next_line = readline("> ");
		if (next_line == NULL)
		{
			free(command);
			return (NULL);
		}
		command = combine_cmd_with_newline(command, next_line);
		if (!command)
			return (NULL);
		if (is_command_complete(command))
			break ;
	}
	return (command);
}
