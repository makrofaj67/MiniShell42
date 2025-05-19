/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_executor.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/20 04:00:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef D_EXECUTOR_H
# define D_EXECUTOR_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <limits.h>
# include <errno.h>
# include <string.h>
# include "c_parser.h"
# include "e_builtins.h"

/*
** Main execution functions
*/
int execute_ast(ast_node *root_node, int *exit_status, 
	t_env **env_list, t_env **export_list);
int execute_ast_node(ast_node *node, int *exit_status,
	t_env **env_list, t_env **export_list);

/*
** Utility functions
*/
int is_builtin(char *cmd);
int execute_builtin(char **args, t_env **env_list, t_env **export_list);
char **env_to_array(t_env *env_list);
char *find_command_in_path(char *cmd, t_env *env_list);
char *path_join(char *dir, char *file);
void free_array(char **array);
char **split_by_char(char *str, char c);
int is_numeric(char *str);
int exit_cmd(char **args);
int ft_atoi(const char *str);

#endif