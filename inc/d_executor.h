/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_executor.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 21:15:42 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 21:44:05 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef D_EXECUTOR_H
# define D_EXECUTOR_H

# include "c_parser.h"
# include <fcntl.h>
# include <sys/stat.h>

/**
 * @brief Maximum number of pipes allowed in a command
 */
# define MAX_PIPES 100

/**
 * @brief Structure to hold execution context
 */
typedef struct s_exec_context {
    char    **envp;         /* Environment variables */
    int     prev_pipe[2];   /* Previous pipe fd */
    int     curr_pipe[2];   /* Current pipe fd */
    int     exit_status;    /* Last command exit status */
    int     is_first_cmd;   /* Is this the first command in pipeline */
    int     is_last_cmd;    /* Is this the last command in pipeline */
    int     pipe_count;     /* Number of pipes in command */
    pid_t   *pids;          /* Array of child process IDs */
    int     heredoc_count;  /* Counter for heredoc files */
} t_exec_context;

/**
 * @brief Initialize execution context structure
 * 
 * @param envp Environment variables array
 * @return t_exec_context* Initialized execution context
 */
t_exec_context *init_exec_context(char **envp);

/**
 * @brief Free execution context and resources
 * 
 * @param context Execution context to free
 */
void free_exec_context(t_exec_context *context);

/**
 * @brief Execute a command based on its AST
 * 
 * @param root_node Root of the AST to execute
 * @param exit_status Pointer to exit status to update
 * @param envp Environment variables array
 * @return int Exit status of the command
 */
int execute_command(ast_node *root_node, int *exit_status, char **envp);

/**
 * @brief Execute a simple command with its arguments and redirections
 * 
 * @param cmd_node AST node containing command details
 * @param context Execution context
 * @return int Exit status of the command
 */
int execute_simple_command(ast_node *cmd_node, t_exec_context *context);

/**
 * @brief Execute a pipeline of commands
 * 
 * @param pipe_node AST node representing a pipeline
 * @param context Execution context
 * @return int Exit status of the last command in pipeline
 */
int execute_pipeline(ast_node *pipe_node, t_exec_context *context);

/**
 * @brief Apply all redirections specified in a command
 * 
 * @param redirections Array of redirection structures
 * @param context Execution context
 * @return int 0 on success, -1 on failure
 */
int apply_redirections(t_redirection **redirections, t_exec_context *context);

/**
 * @brief Find path of executable in PATH environment variable
 * 
 * @param cmd Command name to find
 * @param envp Environment variables
 * @return char* Full path to executable or NULL if not found
 */
char *find_executable(char *cmd, char **envp);

/**
 * @brief Handle a heredoc redirection
 * 
 * @param delimiter The delimiter string that ends the heredoc
 * @param context Execution context
 * @return char* Path to temporary file containing heredoc content
 */
char *handle_heredoc(char *delimiter, t_exec_context *context);

/**
 * @brief Wait for all child processes to complete
 * 
 * @param context Execution context containing process IDs
 * @return int Exit status of the last command
 */
int wait_for_processes(t_exec_context *context);

#endif