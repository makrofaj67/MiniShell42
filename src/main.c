/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:30:36 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:21:36 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/__minishell.h"
#include <unistd.h>

/*
** Primary execution loop for the shell program
** Continuously prompts for and processes user commands
** Handles the read-evaluate-print loop pattern of a command-line interface
** Controls the lifetime of command, token, and execution objects
** 
** @param prompt: The shell prompt string to display to the user
** @param envp: Array of environment variables for command execution
*/
void	shell_loop(char *prompt, char **envp)
{
	char			*command;
	int				should_exit;
	t_token_list	*tokens;
	int             exit_status;

	should_exit = 0;
	exit_status = 0;
	while (true)
	{
		command = get_command(prompt, &should_exit);
		if (command == NULL)
		{
			if (should_exit == 1)
				exit(EXIT_SUCCESS);
			continue ;
		}
		tokens = tokenize_command(command, exit_status);
		print_tokens(tokens);
		free_token_list(tokens);
		free(command);
	}
}

/*
** Entry point for the MiniShell42 program
** Initializes the shell environment, signal handlers, and prompt
** Starts the main command processing loop and performs cleanup on exit
** 
** @param argc: Count of command-line arguments (unused but required by C standard)
** @param argv: Array of command-line argument strings (unused but required by C standard)
** @param envp: Array of environment variable strings passed from the parent process
** @return: Exit status code (0 for normal termination)
*/
int	main(int argc, char **argv, char **envp)
{
	char	*prompt;

	(void)argc;
	(void)argv;
	clear_screen();
	setup_interactive_signals();
	prompt = prepare_prompt(envp);
	shell_loop(prompt, envp);
	free(prompt);
	rl_clear_history();
	return (0);
}
