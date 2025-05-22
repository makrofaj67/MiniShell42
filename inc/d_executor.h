/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_executor.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 01:05:33 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 01:05:33 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef D_EXECUTOR_H
# define D_EXECUTOR_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include "e_builtins.h"

void	execute_path(char *path, char **args);
char	*find_pathx(char *cmd);
int     execute_command_node(ast_node *node, int *exit_status, t_env **env, t_env **env_var);
int     execute_pipe_node(ast_node *node, int *exit_status, t_env **env, t_env **env_var);
int     execute_ast(ast_node *node, int *exit_status, t_env **env, t_env **env_var);
void	execute_command_root(ast_node *root_node, int *exit_status, t_env **env, t_env **env_var);
void    handle_redirections(t_redirection **redirections);
void    heredoc_helper(t_redirection *redirections);

#endif