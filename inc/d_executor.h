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
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include "c_parser.h"
# include "e_builtins.h"
# include "0_core_env.h"
# include "g_libft.h"

typedef struct s_executor_data
{
	t_variable_list	*env_list;
	int				last_exit_status;
	int				pipe_fd[2];
	pid_t			last_pid;
}	t_executor_data;

int		execute_ast(ast_node *root, t_variable_list *env_list, int *exit_status);
int		execute_command_node(ast_node *node, t_executor_data *data);
int		execute_pipe_node(ast_node *node, t_executor_data *data);
int		handle_redirections(command_value *cmd, t_executor_data *data);
int		restore_redirections(command_value *cmd);
int		execute_simple_command(command_value *cmd, t_executor_data *data);
char	*find_command_path(char *cmd, t_variable_list *env_list);
char	**create_env_array(t_variable_list *env_list);
void	free_str_array(char **array);
int		is_builtin(char *cmd);
int		execute_builtin(command_value *cmd, t_executor_data *data);
int		execute_external_command(command_value *cmd, t_executor_data *data);
int		handle_heredoc(t_redirection *redir);
char	*ft_strjoin3(char *s1, char *s2, char *s3);

#endif