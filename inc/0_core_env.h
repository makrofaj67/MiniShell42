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

t_env_exit_status   *init_shell_data(void);

#endif