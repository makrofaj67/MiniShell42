/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:30:36 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 21:59:46 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/__minishell.h"
#include <readline/history.h>
#include <unistd.h>

/*
** Primary execution loop for the shell program
** Continuously prompts for and processes user commands
** Handles the read-evaluate-print loop pattern of a command-line interface
** Controls the lifetime of command, token, and execution objects
** 
** @param envp: Array of environment variables for command execution
*/

void	shell_loop(char **envp)
{
	char			*command;
	int				should_exit;
	t_token_list	*tokens;
	int				exit_status;
	ast_node		*root_node;
	char			*prompt;

	should_exit = 0;
	exit_status = 0;
	while (true)
	{
		// Get a fancy prompt with current status
		prompt = prepare_fancy_prompt(envp, exit_status);
		
		// Set up interactive signal handling for command entry
		setup_interactive_signals();
		
		// Get user command
		command = get_command(prompt, &should_exit);
		free(prompt);
		
		if (command == NULL)
		{
			if (should_exit == 1)
				exit(EXIT_SUCCESS);
			continue;
		}
		
		// Process the command
		tokens = tokenize_command(command, exit_status);
		root_node = parse_tokens(tokens);
		
		if (root_node)
		{
			visualize_ast(root_node);
			
			// Set up execution signal handling
			setup_execution_signals();
			
			// Execute the command
			execute_command(root_node, &exit_status, envp);
			
			// Reset signals back to interactive mode
			setup_interactive_signals();
		}
		
		free_token_list(tokens);
		free_ast(root_node);
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
	(void)argc;
	(void)argv;
	
	// Clear the screen for a fresh start

	
	
	// Display a welcome message
	printf("\033[1;36m");
	printf("╔═════════════════════════════════════════╗\n");
	printf("║            \033[1;33mMiniShell42\033[1;36m                ║\n");
	printf("║                                         ║\n");
	printf("║  \033[0;32mWelcome to your 42 shell implementation\033[1;36m  ║\n");
	printf("║  \033[0;37mType commands and enjoy the features!\033[1;36m    ║\n");
	printf("║  \033[0;37mPress Ctrl+D or type 'exit' to quit\033[1;36m      ║\n");
	printf("╚═════════════════════════════════════════╝\n");
	printf("\033[0m\n");
	
	// Set up signal handlers
	setup_interactive_signals();
	
	// Run the main shell loop
	shell_loop(envp);
	
	// Clean up
	rl_clear_history();
	return (0);
}
