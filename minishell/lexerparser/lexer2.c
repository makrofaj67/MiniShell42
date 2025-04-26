#include "cell.h"
#include "linked_list.c"

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
	else
		return ("UNKNOWN");
}

int	main(int argc, char **argv)
{
	t_token_list	*tokens;
	t_token_node	*current;

	if (argc != 2)
	{
		printf("Usage: %s \"command\"\n", argv[0]);
		return (1);
	}
	tokens = tokenize_command(argv[1]);
	if (!tokens)
	{
		printf("Error: Failed to tokenize command\n");
		return (1);
	}
	printf("Tokens:\n");
	current = tokens->head;
	while (current)
	{
		printf("Type: %s, Value: '%s'\n", 
			token_type_to_string(current->type), current->value);
		current = current->next;
	}
	free_token_list(tokens);
	return (0);
}
