/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_utils.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 04:05:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/20 04:05:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef F_UTILS_H
# define F_UTILS_H

# include <signal.h>
# include <readline/readline.h>
# include <termios.h>
# include <stdio.h>

void	sig_int_handler(int sig);
void	sig_int_heredoc_handler(int sig);
void	setup_interactive_signals(void);
void	setup_exec_signals(void);
void	setup_heredoc_signals(void);
void	reset_signal_flag(void);

void	clear_screen(void);
char	*prepare_prompt();

#endif