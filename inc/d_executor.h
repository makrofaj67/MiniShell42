/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_executor.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/13 18:00:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef D_EXECUTOR_H
# define D_EXECUTOR_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include "c_parser.h"
# include "f_builtins.h"

/*
** Execution functions for commands
*/
void	execute_path(char *path, char **args);
char	*find_pathx(char *cmd);
char	*prepare_path(char *env_path, char *cmd);
void	execute_command(char **args, t_env **env_list, t_env **export_list);
char	*find_command_path(char *command, t_env *env_list);
void	execute_external_command(char **args, t_env *env_list);

/*
** AST execution functions
*/
int	execute_ast(ast_node *node, int *exit_status, t_env **env, t_env **env_var);
void	execute_pipe(ast_node *node, t_env **env_list, t_env **export_list);
int	execute_command_node(ast_node *node, int *exit_status, t_env **env, t_env **env_var);
void	execute_redirection(ast_node *node, t_env **env_list, t_env **export_list);
void	handle_redirection(char **args);

#endif