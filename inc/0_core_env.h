#ifndef CORE_ENV_H
# define CORE_ENV_H

typedef struct s_variable_node
{
	char *key;
	char *value;
	int  is_exported;
	struct s_variable_node *next;
	struct s_variable_node *prev;
} t_variable_node;

typedef struct s_variable_list
{
	t_variable_node *head;
	t_variable_node *tail;
} t_variable_list;

t_variable_list     *init_variable_list(void);
void                load_initial_env(t_variable_list *list, char **envp);
t_variable_node     *find_variable_node(t_variable_list *list, char *key);
char                *get_variable_value(t_variable_list *list, char *key);
void                set_variable(t_variable_list *list, char *key, char *value, int is_exported);
void                unset_variable(t_variable_list *list, char *key);
t_variable_node *create_variable_node(char *key, char *value, int is_exported);
void                free_variable_list(t_variable_list *list);
void list_append_node(t_variable_list *list, t_variable_node *new_node);
void update_existing_variable_node(t_variable_node *node, char *value, int is_exported);
void list_append_node(t_variable_list *list, t_variable_node *new_node);

#endif