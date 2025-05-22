/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_builtins.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:35:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/22 12:35:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef E_BUILTINS_H
# define E_BUILTINS_H

# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <limits.h>
# include <errno.h>
# include "0_core_env.h"
# include "g_libft.h"

int		ft_echo(char **args);
int		ft_cd(char **args, t_variable_list *env_list);
int		ft_pwd(void);
int		ft_export(char **args, t_variable_list *env_list);
int		ft_unset(char **args, t_variable_list *env_list);
int		ft_env(t_variable_list *env_list);
int		ft_exit(char **args, int *exit_status);

#endif