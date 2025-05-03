/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __minishell.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:51:30 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 21:54:14 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MINISHELL_H
# define __MINISHELL_H

# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <ctype.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <signal.h>
# include <limits.h>  /* For PATH_MAX */
# include "a_command.h"
# include "b_lexer.h"
# include "c_parser.h"
# include "d_executor.h"
# include "e_utils.h"
# include "f_builtins.h"
# include "f_libft.h"

/* Global signal flag */
extern volatile sig_atomic_t g_signal_received;

/* Command parsing functions */
char	*get_command(char *prompt, int *should_exit);
int	is_command_blank(char *command);  /* Changed from bool to int to match a_command.h */

/* Signal handling functions */
void	setup_interactive_signals(void);
void	setup_execution_signals(void);
void	reset_signals(void);

/* Prompt and UI functions */
void	clear_screen(void);
char	*prepare_fancy_prompt(char **envp, int exit_status);

/* Shell main loop */
void	shell_loop(char **envp);

/* Command execution */
int		execute_command(ast_node *root_node, int *exit_status, char **envp);

#endif
