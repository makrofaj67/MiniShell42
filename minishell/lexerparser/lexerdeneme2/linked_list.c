typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	ERROR
}	t_token_type;

typedef struct s_token_node
{
	t_token_type		type;
	char				*value;
	struct s_token_node	*next;
	struct s_token_node	*prev;
}	t_token_node;

typedef struct s_token_list
{
	int				size;
	t_token_node	*head;
	t_token_node	*tail;
}	t_token_list;


t_token_list	*init_token_list(void)
{
	t_token_list	*list;

	list = (t_token_list *)malloc(sizeof(t_token_list));
	if (list == NULL)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}

t_token_node	*create_token_node(t_token_type type, char *value)
{
	t_token_node	*node;

	node = (t_token_node *)malloc(sizeof(t_token_node));
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->value = value;
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

void	add_token(t_token_node *node, t_token_list *list)
{
	if (list->size == 0)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		list->tail->next = node;
		node->prev = list->tail;
		list->tail = node;
	}
	list->size++;
}
