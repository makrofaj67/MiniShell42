/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signals.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell <minishell@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 10:00:00 by minishell          #+#    #+#             */
/*   Updated: 2024/07/26 10:00:00 by minishell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

// Global signal flag to track received signals
volatile sig_atomic_t	g_signal_received = 0;

/*
** Signal handler for interactive mode (SIGINT/Ctrl-C)
** Displays a new prompt line, and sets g_signal_received to SIGINT.
*/
void	sig_int_handler(int sig)
{
	(void)sig; // Standard practice if sig parameter is not directly used
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
** Signal handler for heredoc mode (SIGINT/Ctrl-C)
** Exits the heredoc input loop by simulating a newline, sets g_signal_received.
*/
void	sig_int_heredoc_handler(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	// Simulate newline to interrupt readline blocking call (e.g., read())
	// This is a common technique to make readline return.
	if (isatty(STDIN_FILENO))
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0); // Clear the current line content if any
	// rl_on_new_line(); // Usually not needed as ioctl and subsequent logic handles it
}

/*
** Custom signal handler for SIGQUIT.
** This handler prints "Quit (core dumped)" to standard error and sets g_signal_received.
** Note: This handler is provided but not used by the standard setup functions below,
** which either ignore SIGQUIT or set it to its default action (SIG_DFL).
*/
void	sig_quit_handler(int sig)
{
	(void)sig;
	g_signal_received = SIGQUIT;
	write(STDERR_FILENO, "Quit (core dumped)\n", 19);
}

/*
** Resets the global signal flag.
** Call this after a signal has been processed by the main loop.
*/
void	reset_signal_flag(void)
{
	g_signal_received = 0;
}

/*
** Setup signal handlers for interactive mode.
** - SIGINT (Ctrl-C): Handled by sig_int_handler.
** - SIGQUIT (Ctrl-\): Ignored.
*/
void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sig_int_handler;
	sa_int.sa_flags = SA_RESTART; // Restart syscalls if interrupted by this signal
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_IGN; // Ignore SIGQUIT
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/*
** Setup signal handlers for command execution (typically for child processes).
** - SIGINT (Ctrl-C): Default behavior (terminate process).
** - SIGQUIT (Ctrl-\): Default behavior (terminate process and core dump).
*/
void	setup_exec_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = SIG_DFL; // Default action for SIGINT
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_DFL; // Default action for SIGQUIT
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/*
** Setup signal handlers for heredoc input mode.
** - SIGINT (Ctrl-C): Handled by sig_int_heredoc_handler to interrupt input.
** - SIGQUIT (Ctrl-\): Ignored, as in interactive mode.
*/
void	setup_heredoc_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sig_int_heredoc_handler;
	// Do not set SA_RESTART for heredoc SIGINT, so read() is interrupted.
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_IGN; // Ignore SIGQUIT
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}
