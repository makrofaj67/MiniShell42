/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 23:00:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

int	builtin_echo(char **argv);
int	builtin_cd(char **argv, t_env **env);
int	builtin_pwd(void);
int	builtin_export(char **argv, t_env **env);
int	builtin_unset(char **argv, t_env **env);
int	builtin_env(t_env *env);
int	builtin_exit(char **argv);
int	is_builtin(const char *cmd);
int	exec_builtin(char **argv, t_env **env);
char	*find_in_path(const char *cmd, t_env *env);

#endif
