/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 05:20:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/20 05:20:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Global signal flag to track received signals
*/
volatile sig_atomic_t	g_signal_received = 0;

/*
** Signal handler for interactive mode (SIGINT/Ctrl-C)
** Displays a new prompt line
*/
void	sig_int_handler(int sig)
{
	g_signal_received = sig;
	g_exit_status = 130;  // Standard Ctrl-C exit code
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
** Signal handler for heredoc mode (SIGINT/Ctrl-C)
** Exits the heredoc input loop
*/
void	sig_int_heredoc_handler(int sig)
{
	g_signal_received = sig;
	g_exit_status = 130;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

/*
** Setup signal handlers for interactive mode
** Handles SIGINT (Ctrl-C) and SIGQUIT (Ctrl-\)
*/
void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sig_int_handler;
	sa_int.sa_flags = SA_RESTART;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_IGN;  // Ignore SIGQUIT
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/*
** Setup signal handlers for child process execution mode
** Uses default handlers for both SIGINT and SIGQUIT
*/
void	setup_exec_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = SIG_DFL;  // Default SIGINT behavior
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_DFL;  // Default SIGQUIT behavior
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/*
** Setup signal handlers for heredoc input mode
** Handles SIGINT (Ctrl-C) to exit the heredoc input loop
*/
void	setup_heredoc_signals(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = sig_int_heredoc_handler;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
}

/*
** Reset signal flag after it's been handled
*/
void	reset_signal_flag(void)
{
	g_signal_received = 0;
}
