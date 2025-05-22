#include "../../inc/__minishell.h"

static int	check_special_chars(char c)
{
	if (c == ' ' || c == '\t' || c == '/' || c == '='
		|| c == ':' || c == ';' || c == ',' || c == '.')
		return (1);
	return (0);
}

static int	check_alphanumeric(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
		return (1);
	return (0);
}

static int	check_escape_sequence(char *raw_command, int i)
{
	int	quote_state;

	if (i > 0 && raw_command[i - 1] == '\\')
	{
		quote_state = get_quote_state_at_position(raw_command, i - 1);
		if (quote_state == 0 || quote_state == 2)
			return (1);
	}
	return (0);
}

int	is_need_for_expanding(char *raw_command, int i)
{
	int	quote_state;

	if (check_escape_sequence(raw_command, i) == 1)
		return (0);
	if (raw_command[i + 1] == '\0' || raw_command[i + 1] == '$')
		return (0);
	if (check_special_chars(raw_command[i + 1]) == 1)
		return (0);
	quote_state = get_quote_state_at_position(raw_command, i);
	if (quote_state == 1)
		return (0);
	if (raw_command[i + 1] == '?')
		return (1);
	if (check_alphanumeric(raw_command[i + 1]) == 1)
		return (1);
	return (0);
}
