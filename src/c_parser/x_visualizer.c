#include "../../inc/__minishell.h"

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


