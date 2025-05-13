/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_executor.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:36:12 by rakman            #+#    #+#             */
/*   Updated: 2025/05/13 17:20:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef D_EXECUTOR_H
# define D_EXECUTOR_H

# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include "f_builtins.h"

/**
 * External command execution functions from a_executor.c
 */
void    execute_command(char **args, t_env **env_list, t_env **export_list);
char    *find_command_path(char *command, t_env *env_list);
void    execute_external_command(char **args, t_env *env_list);
void    handle_redirection(char **args);

/**
 * AST execution functions (to be added)
 */
// void    execute_ast(ast_node *node, t_env **env_list, t_env **export_list);
// void    execute_pipe(ast_node *node, t_env **env_list, t_env **export_list);
// void    execute_command_node(ast_node *node, t_env **env_list, t_env **export_list);
// void    execute_redirection(ast_node *node, t_env **env_list, t_env **export_list);

# endif