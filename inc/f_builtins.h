/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_builtins.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/13 18:00:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef F_BUILTINS_H
# define F_BUILTINS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/*
** Environment variable structure
*/
typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env *next;
}				t_env;

/*
** String utilities
*/
int		mini_strcmp_path(char *path, char *cmd);
char	*mini_strcat_p(char *dest, char *src);
char	*mini_strcat(char *dest, char *src);
int		mini_strlen(char *s);
t_env	*mini_lstlast(t_env *lst);

/*
** Environment variable management
*/
int		is_variable(char **args);
void	env_lengths(t_env **len, char **args);
void	add_variable(char **args, t_env **env);
void	add_list_variable(char **args, t_env **env);
void	env_value(char *args, t_env *env, int n);
void	print_nodes(t_env **tmp);
void	create_env(t_env **env);
void	add_variable_2(char *args, t_env **env);
void	env_lengths_2(t_env **len, char *args);
void	add_env_var(char *key, char *value, t_env **env);

/*
** Built-in commands
*/
void	pwd_cmd(void);
void	cd_cmd(char **args);
void	echo_cmd(char **args, t_env **env);
void	export_cmd(char *arg, t_env **env, t_env **env_var);
void	unset_cmd(char *args, t_env **env);
int		builtin_selector(char **args, t_env **env, t_env **env_var);

/*
** Path utilities
*/
char	*get_path_home(void);

#endif