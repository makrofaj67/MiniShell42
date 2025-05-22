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

typedef	struct	s_env
{
	char	*key;
	char	*value;
	struct s_env *next;
}				t_env;

void free_env_list(t_env **env);
void	unset_cmd(char *args, t_env **env);
void	create_env(t_env **env);
void	export_cmd(char *arg, t_env **env, t_env **env_var);
void	add_env_var(char *key, char *value, t_env **env);
void	add_list_variable(char **args, t_env **env);
void	add_variable_2(char *args, t_env **env);
void	env_lengths_2(t_env **len, char *args);
t_env	*mini_lstlast(t_env *lst);
void	add_variable(char **args, t_env **env);
void	env_lengths(t_env **len, char **args);
void	echo_cmd(char **args, t_env **env);
int		mini_strcmp_path(char *path, char *cmd);
char	*mini_strcat_p(char *dest, char *src);
char 	*mini_strcat(char *dest, char *src);
int 	mini_strlen(char *s);
t_env	*mini_lstlast(t_env *lst);
void	cd_cmd(char **args);
void	pwd_cmd(void);
int		builtin_selector(char **args, t_env **env, t_env **env_var);
int		is_variable(char **args);
void	print_nodes(t_env **tmp);

#endif