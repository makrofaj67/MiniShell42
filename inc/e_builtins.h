/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_builtins.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 01:45:23 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 01:45:23 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef F_BUILTINS_H
# define F_BUILTINS_H

# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include "0_core_env.h" // Include the new environment variable management header

// void free_env_list(t_env **env); // Remove old t_env related functions
void	unset_cmd(char *arg_name, t_variable_list *variables);
// void	create_env(t_env **env); // Remove old t_env related functions
void	export_cmd(char **args, t_variable_list *variables);
// void	add_env_var(char *key, char *value, t_env **env); // Remove old t_env related functions
// void	add_list_variable(char **args, t_env **env); // Remove old t_env related functions
// void	add_variable_2(char *args, t_env **env); // Remove old t_env related functions
// void	env_lengths_2(t_env **len, char *args); // Remove old t_env related functions
// t_env	*mini_lstlast(t_env *lst); // Remove old t_env related functions
// void	add_variable(char **args, t_env **env); // Remove old t_env related functions
// void	env_lengths(t_env **len, char **args); // Remove old t_env related functions
void	echo_cmd(char **args, t_variable_list *variables, int current_exit_status);
int		mini_strcmp_path(char *path, char *cmd);
char	*mini_strcat_p(char *dest, char *src);
char 	*mini_strcat(char *dest, char *src);
int 	mini_strlen(char *s);
// t_env	*mini_lstlast(t_env *lst); // Remove old t_env related functions
void	cd_cmd(char **args, t_variable_list *variables);
void	pwd_cmd(void);
int		builtin_selector(char **args, t_variable_list *variables, int *current_exit_status);
// int		is_variable(char **args); // This logic will be handled by export/set_variable
// void	print_nodes(t_env **tmp); // Remove old t_env related functions
void	print_exported_vars(t_variable_list *variables); // For env command
void	print_all_vars_sorted(t_variable_list *variables); // For export command with no args

#endif