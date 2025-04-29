/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_executer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:15:18 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 13:54:14 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef D_EXECUTER_H
# define D_EXECUTER_H

# include "c_parser.h"
# include <sys/types.h>

# define READ_END 0
# define WRITE_END 1

typedef struct s_exec_context
{
	char	**env;
	int		last_exit_status;
	int		pipe_fd[2];
	pid_t	last_pid;
}	t_exec_context;

/* Main execution function - Execute a complete AST tree */
int		execute_ast(t_ast_node *ast, t_exec_context *ctx);

/* Executor helper functions for different node types */
int		execute_command(t_ast_node *ast, t_exec_context *ctx);
int		execute_pipe(t_ast_node *ast, t_exec_context *ctx);
int		execute_and(t_ast_node *ast, t_exec_context *ctx);
int		execute_or(t_ast_node *ast, t_exec_context *ctx);
int		execute_group(t_ast_node *ast, t_exec_context *ctx);

/* Redirection handling */
int		setup_redirections(t_redir *redirs);
int		restore_std_fds(int stdin_copy, int stdout_copy);

/* Environment variable handling */
char	*expand_env_vars(char *arg, char **env);
char	**copy_env(char **env);
char	*get_env_var(char *name, char **env);

/* Context management */
t_exec_context	*init_exec_context(char **env);

#endif