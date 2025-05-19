#include "../../inc/__minishell.h"

static int	handle_quote_state(char *command, int *i, int *quote_state)
{
	if (*quote_state == 0 && command[*i] == '\'')
		*quote_state = 1;
	else if (*quote_state == 0 && command[*i] == '"')
		*quote_state = 2;
	else if (*quote_state == 1 && command[*i] == '\'')
		*quote_state = 0;
	else if (*quote_state == 2 && command[*i] == '"')
		*quote_state = 0;
	else
		return (1);
	return (0);
}

char	*get_quote_trimmed(char *command)
{
	size_t	command_len;
	char	*concated_str;
	int		i;
	int		j;
	int		quote_state;

	if (!command)
		return (NULL);
	command_len = ft_strlen(command);
	concated_str = (char *)malloc(sizeof(char) * (command_len + 1));
	if (!concated_str)
		return (NULL);
	i = 0;
	j = 0;
	quote_state = 0;
	while (command[i])
	{
		if (handle_quote_state(command, &i, &quote_state) == 1)
			concated_str[j++] = command[i];
		i++;
	}
	concated_str[j] = '\0';
	return (concated_str);
}
