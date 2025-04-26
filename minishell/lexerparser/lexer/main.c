#include "tokenizer.c"

void	print_tokens(t_token_list *list)
{
	t_token_node	*current;

	printf("Tokens:\n");
	current = list->head;
	while (current)
	{
		printf("Type: %s, Value: '%s'\n", 
			token_type_to_string(current->type), current->value);
		current = current->next;
	}
}

int	main(int argc, char **argv)
{
	t_token_list	*tokens;

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
	print_tokens(tokens);
	free_token_list(tokens);
	return (0);
}
