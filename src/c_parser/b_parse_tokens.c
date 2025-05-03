/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_parse_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:39:17 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 20:13:26 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <stdlib.h>

t_token_list *get_tokens_after(t_token_list *tokens, t_token_node *pipe_location)
{
	t_token_list *new_list;
    t_token_node *current_original;
	
	new_list = create_token_list();
    if (new_list == NULL) 
	{
        perror("Failed to create new token list in get_tokens_before");
        return NULL;
    }
	current_original = pipe_location->next;
    while (current_original != NULL && current_original != pipe_location) 
	{
        add_token(new_list, current_original->value);
        current_original = current_original->next;
    }
    return new_list;
}


t_token_list *get_tokens_before(t_token_list *tokens, t_token_node *pipe_location) 
{
	t_token_list *new_list;
    t_token_node *current_original;
	
	new_list = create_token_list();
    if (new_list == NULL) 
	{
        perror("Failed to create new token list in get_tokens_before");
        return NULL;
    }
	current_original = tokens->head;
    while (current_original != NULL && current_original != pipe_location) 
	{
        add_token(new_list, current_original->value);
        current_original = current_original->next;
    }
    return new_list;
}

typedef struct s_generic_list_node {
    void *value;
    struct s_generic_list_node *prev;
    struct s_generic_list_node *next;
} t_gl_node;

typedef struct s_generic_list {
	int			size;
	t_gl_node *head;
	t_gl_node *tail;
} t_generic_list;

t_generic_list *init_generic_list(void)
{
	t_generic_list *gn_list;
	gn_list = (t_generic_list *)malloc(sizeof(t_generic_list));
	if (gn_list == NULL)
		return (NULL);
	gn_list->head = NULL;
	gn_list->tail = NULL;
	gn_list->size = 0;
	return (gn_list);
}

int add_node_to_generic_list(t_generic_list *list, void *value)
{
    t_gl_node *new_node = (t_gl_node *)malloc(sizeof(t_gl_node));
    if (new_node == NULL) 
        return (0);    
	new_node->value = value; 
    new_node->next = NULL;
	if (list->tail == NULL) 
	{
        new_node->prev = NULL;
        list->head = new_node;
        list->tail = new_node;
    } 
	else 
	{
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
	list->size++;
	return (1); // Başarılı oldu
}

void add_redir(t_token_node *node, t_generic_list *list) 
{
    t_redirection	*new_redir;
	int				success;
	
	new_redir = (t_redirection *)malloc(sizeof(t_redirection));
    if (new_redir == NULL) {
        perror("malloc failed for t_redirection in add_redir");
        return;
    }
	switch (node->type) 
	{
    case RDRT_IN:
        new_redir->type = REDIR_IN;
        break;
    case RDRT_OUT:
        new_redir->type = REDIR_OUT;
        break;
    case APPEND:
        new_redir->type = REDIR_APPEND;
        break;
    case HEREDOC:
        new_redir->type = REDIR_HEREDOC;
        break;
    default:
        break;
	}
    new_redir->filename = strdup(node->next->value); 
    if (new_redir->filename == NULL) {
        perror("strdup failed for filename in add_redir");
        free(new_redir);
        return; 
    }
    success = add_node_to_generic_list(list, new_redir);
    if (!success) 
	{
        perror("add_node_to_generic_list failed in add_redir");
        free(new_redir->filename);
        free(new_redir);
        return;
    }
}

void add_word(t_token_node *node, t_generic_list *list)
{
	char *word_copy;
	int success;

	word_copy = strdup(node->value);
    if (word_copy == NULL) 
	{
        perror("strdup failed in add_word"); 
        return;
    }
    success = add_node_to_generic_list(list, word_copy);
    if (success == 0) 
	{
        free(word_copy); 
        perror("add_node_to_generic_list failed in add_word");
        return;
    }
}

void free_generic_list_nodes_only(t_generic_list *list) {
    
	t_gl_node *next_node;
	t_gl_node *current; 
	
	if (list == NULL) 
		return;
	current = list->head;
	while (current != NULL) 
	{
		next_node = current->next; 
		free(current); 
		current = next_node; 
	}
	free(list);
}

command_value *parse_simple_command(t_token_list *tokens)
{
	command_value		*cmd_details;
	t_generic_list		*redir_list;
	t_generic_list		*word_list;
	t_token_node		*current_token;

	cmd_details = (command_value *)malloc(sizeof(command_value));
	if (cmd_details == NULL)
		return (NULL);
	redir_list = init_generic_list();
	word_list = init_generic_list();
	current_token = tokens->head;
	while(current_token != NULL)
	{
		if (current_token->type == RDRT_IN || current_token->type == RDRT_OUT || current_token->type == HEREDOC || current_token->type == APPEND)
		{
			if (current_token->next == NULL || current_token->next->type != WORD)
			{
				printf("Syntax Error");
				return (NULL);
			}
			else
			{
				add_redir(current_token, redir_list);
				if (current_token->next != NULL)
					current_token = current_token->next->next;
			}
		}
		else if (current_token->type == WORD)
		{
			add_word(current_token, word_list);
			current_token = current_token->next;
		}
		else
		{
			printf("Unknown error");
			return (NULL);
		}
	}

	cmd_details->arg_array = (char **)malloc(sizeof(char *) * (word_list->size + 1));
	if (cmd_details->arg_array == NULL)
		return (NULL);
	int i;
	i = 0;
	t_gl_node *current_word;
	current_word = word_list->head;
	while(current_word != NULL)
	{
		cmd_details->arg_array[i++] = current_word->value;
		current_word = current_word->next;
	}
	cmd_details->arg_array[i] = NULL;

	cmd_details->redirections = (t_redirection **)malloc(sizeof(t_redirection *) * (redir_list->size + 1));
	if (cmd_details->redirections == NULL)
		return (NULL);
	int j;
	j = 0;
	t_gl_node *current_redir;
	current_redir = redir_list->head;
	while(current_redir != NULL)
	{	
		cmd_details->redirections[j] = (t_redirection *)current_redir->value; //void için castingi bizim yapmamız lazım
		j++;
		current_redir = current_redir->next;
	}
	cmd_details->redirections[j] = NULL; // Add NULL terminator to mark the end of redirections array

	free_generic_list_nodes_only(redir_list);
	free_generic_list_nodes_only(word_list);
	return (cmd_details);
}

ast_node *create_command_node(command_value *cmd_details)
{
	ast_node *command_node;
	command_node = (ast_node *)malloc(sizeof(ast_node));
	if (command_node == NULL)
		return (NULL);
	command_node->type = COMMAND_NODE;
	command_node->left = NULL;
	command_node->right = NULL;
	command_node->value = cmd_details;
	return(command_node);
}

t_token_node *find_last_pipe(t_token_list *tokens)
{
	t_token_node *current;
	if (tokens == NULL || tokens->head == NULL || tokens->tail == NULL)
		return (NULL);
	current = tokens->tail;
	while (current != NULL)
	{
		if (current->type == PIPE)
			return (current);
		current = current->prev;
	}
	return (NULL);
}

ast_node *create_pipe_node(ast_node *left_child, ast_node *right_child)
{
	ast_node *pipe_node;
	pipe_node = (ast_node *)malloc(sizeof(ast_node));
	if (pipe_node == NULL)
		return (NULL);
	pipe_node->type = PIPE_NODE;
	pipe_node->left = left_child;
	pipe_node->right = right_child;
	pipe_node->value = NULL;
	return(pipe_node);
}

void free_command_value(command_value *details) {
    int i;
    if (details == NULL) {
        return;
    }
    if (details->arg_array != NULL) {
        i = 0;
        while (details->arg_array[i] != NULL) {
            free(details->arg_array[i]);
            i++;
        }
        free(details->arg_array);
    }
    if (details->redirections != NULL) {
        i = 0;
        while (details->redirections[i] != NULL) {
            if (details->redirections[i]->filename != NULL) {
                free(details->redirections[i]->filename);
            }
            free(details->redirections[i]);
            i++;
        }
        free(details->redirections);
    }
    free(details);
}

void free_ast(ast_node *node) {
    if (node == NULL) {
        return;
    }
    switch (node->type) {
        case COMMAND_NODE:
            free_command_value(node->value);
            break;

        case PIPE_NODE:
            free_ast(node->left);
            free_ast(node->right);
            break;
    }
    free(node);
}

/**
 * @brief Visualizes a redirection with ASCII art
 * 
 * @param redir The redirection to visualize
 * @param redir The redirection to visualize
 */
void visualize_redirection(t_redirection *redir) {
    char *redir_symbol;
    
    switch (redir->type) {
        case REDIR_IN:
            redir_symbol = "<";
            break;
        case REDIR_OUT:
            redir_symbol = ">";
            break;
        case REDIR_APPEND:
            redir_symbol = ">>";
            break;
        case REDIR_HEREDOC:
            redir_symbol = "<<";
            break;
        default:
            redir_symbol = "?";
            break;
    }
    
    printf(" %s %s", redir_symbol, redir->filename);
}

/**
 * @brief Creates a formatted indentation string for tree visualization
 * 
 * @param depth Current depth level in the tree
 * @param is_last Whether this is the last child at this level
 * @param prefix The prefix string to build upon
 * @return char* Formatted indentation string
 */
char *get_tree_branch(int depth, int is_last, char *prefix) {
    char *new_prefix;
    char *branch;
    size_t needed_size;
    
    if (depth == 0)
        return strdup("");
    
    // Calculate the size needed for the new prefix: length of prefix + branch + null terminator
    branch = is_last ? "└── " : "├── ";
    needed_size = strlen(prefix) + strlen(branch) + 1;
    
    new_prefix = malloc(needed_size);
    if (!new_prefix)
        return strdup("");
    
    // Use snprintf for safer string formatting with size limit
    snprintf(new_prefix, needed_size, "%s%s", prefix, branch);
    return new_prefix;
}

/**
 * @brief Creates the prefix for the next level of the tree
 * 
 * @param depth Current depth level in the tree
 * @param is_last Whether this is the last child at this level
 * @param prefix The current prefix string
 * @return char* Prefix for the next level
 */
char *get_next_prefix(int depth, int is_last, char *prefix) {
    char *next_prefix;
    char *extension;
    size_t needed_size;
    
    if (depth == 0)
        return strdup("");
    
    // Calculate size needed: length of prefix + extension + null terminator
    extension = is_last ? "    " : "│   ";
    needed_size = strlen(prefix) + strlen(extension) + 1;
    
    next_prefix = malloc(needed_size);
    if (!next_prefix)
        return strdup("");
    
    // Use snprintf for safer string formatting with size limit
    snprintf(next_prefix, needed_size, "%s%s", prefix, extension);
    return next_prefix;
}

/**
 * @brief Recursively visualizes the AST starting from the given node
 * 
 * @param node The AST node to visualize
 * @param depth Current depth in the tree
 * @param is_last Whether this node is the last child of its parent
 * @param prefix Current prefix for formatting
 */
void visualize_ast_recursive(ast_node *node, int depth, int is_last, char *prefix) {
    char *branch;
    char *next_prefix;
    int i;
    
    if (node == NULL)
        return;
    
    branch = get_tree_branch(depth, is_last, prefix);
    
    if (node->type == PIPE_NODE) {
        printf("%s\033[1;36mPIPE\033[0m\n", branch);
        
        next_prefix = get_next_prefix(depth, is_last, prefix);
        
        // Visualize left child (command before pipe)
        visualize_ast_recursive(node->left, depth + 1, 0, next_prefix);
        
        // Visualize right child (command after pipe)
        visualize_ast_recursive(node->right, depth + 1, 1, next_prefix);
        
        free(next_prefix);
    } else if (node->type == COMMAND_NODE) {
        if (node->value && node->value->arg_array && node->value->arg_array[0]) {
            printf("%s\033[1;32mCOMMAND\033[0m: ", branch);
            
            // Print command arguments
            i = 0;
            while (node->value->arg_array[i]) {
                if (i > 0)
                    printf(" ");
                printf("\033[1;33m%s\033[0m", node->value->arg_array[i]);
                i++;
            }
            
            // Print redirections if any
            if (node->value->redirections) {
                i = 0;
                while (node->value->redirections[i]) {
                    visualize_redirection(node->value->redirections[i]);
                    i++;
                }
            }
            printf("\n");
        } else {
            printf("%s\033[1;31mEMPTY COMMAND\033[0m\n", branch);
        }
    }
    
    free(branch);
}

/**
 * @brief Visualizes the AST in a pretty tree format with colors
 * 
 * @param root The root node of the AST to visualize
 */
void visualize_ast(ast_node *root) {
    if (root == NULL) {
        printf("\033[1;31mEmpty AST (NULL)\033[0m\n");
        return;
    }
    
    printf("\n\033[1;35m===== AST Visualization =====\033[0m\n\n");
    visualize_ast_recursive(root, 0, 1, "");
    printf("\n\033[1;35m============================\033[0m\n\n");
}

ast_node *parse_tokens(t_token_list *tokens) {

	if (tokens == NULL || tokens->head == NULL)
	    return NULL;

	t_token_node *pipe_location = find_last_pipe(tokens);
    if (pipe_location != NULL)
    {
        t_token_list *left_tokens = get_tokens_before(tokens, pipe_location);
        t_token_list *right_tokens = get_tokens_after(tokens, pipe_location);
        if (left_tokens == NULL || right_tokens == NULL) {
             fprintf(stderr, "Error creating sub token lists\n");
             free_token_list(left_tokens);
             free_token_list(right_tokens);
             return NULL;
        }
        ast_node *left_child = parse_tokens(left_tokens);
        ast_node *right_child = parse_tokens(right_tokens);
        free_token_list(left_tokens);
        free_token_list(right_tokens);
        if (left_child == NULL || right_child == NULL) {
             fprintf(stderr, "Error parsing sub-commands for pipe\n");
             free_ast(left_child);  // Eğer left_child NULL ise free_ast bir şey yapmaz
             free_ast(right_child); // Eğer right_child NULL ise free_ast bir şey yapmaz
             return NULL; // AST oluşturma başarısız
        }
        ast_node *pipe_node = create_pipe_node(left_child, right_child);
        if (pipe_node == NULL) {
            fprintf(stderr, "Error creating pipe node\n");
            free_ast(left_child);
            free_ast(right_child);
            return NULL;
        }
        return pipe_node; // Başarılı
    }
    else // Pipe bulunamadı -> Basit komut durumu
    {
        command_value *cmd_details = parse_simple_command(tokens);
        if (cmd_details == NULL) {
            return NULL;
        }
        ast_node *command_node = create_command_node(cmd_details);
        if (command_node == NULL) {
             fprintf(stderr, "Error creating command node\n");
             free_command_value(cmd_details);
             return NULL;
        }
        return command_node; // Başarılı
    }
}

