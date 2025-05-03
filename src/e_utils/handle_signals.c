/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 22:25:32 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 21:54:14 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <signal.h>
#include <termios.h>
#include <readline/readline.h>

// Global signal flag for tracking received signals
volatile sig_atomic_t g_signal_received = 0;

/**
 * @brief Signal handler for SIGINT (Ctrl+C) in interactive mode
 * 
 * Displays a new prompt on a new line
 * 
 * @param sig Signal number
 */
static void	sigint_handler(int sig)
{
	(void)sig;
	g_signal_received = 1;
	
	// Print a newline
	write(STDOUT_FILENO, "\n", 1);
	
	// Clear the current line
	rl_on_new_line();
	rl_replace_line("", 0);
	
	// Display the prompt again
	rl_redisplay();
}

/**
 * @brief Signal handler for SIGINT in command execution mode
 * 
 * Just sets the global flag and allows parent to handle properly
 * 
 * @param sig Signal number
 */
static void	sigint_exec_handler(int sig)
{
	(void)sig;
	g_signal_received = 1;
	write(STDOUT_FILENO, "\n", 1);
}

/**
 * @brief Ignore SIGQUIT (Ctrl+\) signals
 * 
 * @param sig Signal number
 */
static void	sigquit_handler(int sig)
{
	(void)sig;
	// Do nothing, just avoid the default behavior (core dump)
}

/**
 * @brief Setup signal handlers for interactive mode (command entry)
 */
void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct termios		term;

	// Initialize sigaction structs
	sa_int.sa_handler = sigint_handler;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	
	sa_quit.sa_handler = sigquit_handler;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);

	// Set signal handlers
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
	
	// Configure terminal to disable Ctrl+C echo character (^C)
	if (isatty(STDIN_FILENO))
	{
		tcgetattr(STDIN_FILENO, &term);
		term.c_lflag &= ~ECHOCTL;  // Disable control characters like ^C from showing
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
}

/**
 * @brief Setup signal handlers for command execution mode
 */
void	setup_execution_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	// Initialize sigaction structs
	sa_int.sa_handler = sigint_exec_handler;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	
	sa_quit.sa_handler = SIG_IGN;  // Ignore SIGQUIT completely
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);

	// Set signal handlers
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/**
 * @brief Reset signal handlers to default behavior
 */
void	reset_signals(void)
{
	struct sigaction	sa;
	struct termios		term;

	// Reset to default handlers
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	
	// Reset terminal settings
	if (isatty(STDIN_FILENO))
	{
		tcgetattr(STDIN_FILENO, &term);
		term.c_lflag |= ECHOCTL;  // Re-enable control characters
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
}
