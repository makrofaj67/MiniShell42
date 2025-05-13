/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_builtins.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:36:12 by rakman            #+#    #+#             */
/*   Updated: 2025/05/13 16:36:12 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef F_BUILTINS_H
# define F_BUILTINS_H

# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

// Definition of t_env structure
typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env *next;
}				t_env;

// Utility functions (from builtin_tools.c)
int		mini_strcmp_path(const char *s1, const char *s2);
char	*mini_strcat_p(char *dest, const char *src_path_segment, const char *src_command);
size_t	mini_strlen(const char *s);
char	*mini_strcat(char *dest, const char *src);
t_env	*mini_lstlast(t_env *lst);

// Environment variable functions (from add_variable.c, env.c)
int		is_variable(const char *str);
void	env_lengths(t_env *env, int *count_vars, int *max_len);
void	add_variable(const char *arg, t_env **env_list);
void	add_list_variable(char **args, t_env **env);
char	*env_value(const char *name, t_env *env_list);
void	print_nodes(t_env *env_list);
void	create_env(t_env **env);
void	add_variable_2(char *args, t_env **env);
void	env_lengths_2(t_env **len, char *args);
void	add_env_var(char *key, char *value, t_env **env);

// Builtin commands
int		pwd_cmd(void);                                        // from pwd.c
int		cd_cmd(char **args, t_env **env_list);               // from cd.c
int		echo_cmd(char **args);                               // from echo.c
int		export_cmd(char *arg, t_env **env, t_env **env_var); // from export.c
int		unset_cmd(char *args, t_env **env);                  // from unset.c
int		builtin_selector(char **args, t_env **env_list, t_env **export_list); // from selector.c

// Path functions
char	*get_path_home(void); // from cd.c

#endif