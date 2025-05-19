/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __minishell.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:51:30 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 15:37:36 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MINISHELL_H
# define __MINISHELL_H

# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <ctype.h>
# include <stdbool.h>

# include "a_command.h"
# include "b_lexer.h"
# include "c_parser.h"
# include "d_executor.h"
# include "e_utils.h"
# include "g_utils.h"
# include "f_libft.h"
# include "e_builtins.h"

/* Global variables */
extern volatile sig_atomic_t	g_signal_received;  /* Signal handling flag */
extern int                     g_exit_status;     /* Exit status of last command */
extern t_env                   *g_env_list;       /* Global environment list */
extern char                    **environ;         /* System environment variable */

#endif
