#include "cell.h"
#include "linked_list.c"

int main(int argc, char **argv)
{
	t_token_stack *stack;
	char *command;
	int i;

	if (argc != 2)
		printf("need exactly 2 arguments");
	
	i = 0;
	command = argv[1];
	stack = init_token_stack();
	while(isspace(command[i]))
		i++;
	char *start;
	t_token_node *node;
	while(command[i])
	{
		start = command + i;
		if (isspace(command[i + 1]) || command[i + 1] == '\0')
		{
			node = create_token_node(command[i]);
		}
	}
}	
