/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_collect_multiple_line.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:54:26 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 20:35:16 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/* Reference the global signal variable */
extern volatile sig_atomic_t	g_signal_received;

/* Checks quote context for a position in a string */
int	check_quotes(char *str, int pos)
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
char	*trim_trailing_spaces(char *str)
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
int	get_continuation_char(const char *str)
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
int	is_quote_start(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '"' || str[i] == '\'')
		return (1);
	return (0);
}

/* Removes trailing backslash */
char	*remove_trailing_backslash(char *cmd)
{
	int	len;

	len = strlen(cmd);
	if (len > 0 && cmd[len - 1] == '\\')
		cmd[len - 1] = '\0';
	return (cmd);
}
