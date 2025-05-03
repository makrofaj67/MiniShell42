/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_get_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 22:35:19 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 21:54:14 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

/**
 * @brief Maximum items in completion list
 */
#define MAX_COMPLETIONS 500

/**
 * @brief History file path
 */
#define HISTORY_FILE ".minishell_history"

/**
 * @brief Global variable for completion state
 */
static char *g_completions[MAX_COMPLETIONS];
static int g_completion_count = 0;

/**
 * @brief Custom readline completion function for file paths
 * 
 * @param text The text to complete
 * @param start Start position of text in rl_line_buffer
 * @param end End position
 * @return char** List of completion matches
 */
static char **custom_completion(const char *text, int start, int end)
{
	// This disables default filename completion
	rl_attempted_completion_over = 1;
	
	// If not at the beginning of line and previous char is not space, 
	// use custom completion
	if (start > 0 && !isspace(rl_line_buffer[start - 1]))
		return (NULL);
	
	return (rl_completion_matches(text, rl_filename_completion_function));
}

/**
 * @brief Initialize readline settings, history, and completion
 */
static void initialize_readline(void)
{
	char *home_dir;
	char history_path[1024];
	
	// Set up custom completion
	rl_attempted_completion_function = custom_completion;
	
	// Load history from file
	home_dir = getenv("HOME");
	if (home_dir)
	{
		snprintf(history_path, sizeof(history_path), "%s/%s", home_dir, HISTORY_FILE);
		read_history(history_path);
	}
}

/**
 * @brief Save command history to file
 */
static void save_history(void)
{
	char *home_dir;
	char history_path[1024];
	
	home_dir = getenv("HOME");
	if (home_dir)
	{
		snprintf(history_path, sizeof(history_path), "%s/%s", home_dir, HISTORY_FILE);
		write_history(history_path);
	}
}

/**
 * @brief Get user input using readline with enhanced features
 * 
 * @param prompt The prompt to display
 * @param should_exit Pointer to flag indicating if user wants to exit
 * @return char* The command entered by the user
 */
char *get_command(char *prompt, int *should_exit)
{
	char *command;
	static int first_call = 1;
	
	// Initialize readline on first call
	if (first_call)
	{
		initialize_readline();
		first_call = 0;
	}
	
	// Reset signal handler for each new command
	if (g_signal_received)
	{
		g_signal_received = 0;
		*should_exit = 0;
	}
	
	// Get command using readline
	command = readline(prompt);
	
	// Check for EOF (Ctrl+D)
	if (!command)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		*should_exit = 1;
		save_history();
		return (NULL);
	}
	
	// Skip empty commands from history
	if (*command)
	{
		add_history(command);
		save_history();
	}
	
	return (command);
}
