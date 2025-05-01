/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:30:36 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 13:54:14 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/__minishell.h"

/*
** The main shell loop
*/
void	shell_loop(char *prompt, char **envp)
{
	char			*command;

	while (true)
	{
		command = get_command(prompt);
		if (command == NULL)
			continue ;

		printf("\n\n-->%s<--\n", command);
		// Parsing - convert tokens to AST
		/*ast = parse(lexed_list);
		if (!ast)
		{
			printf("minishell: syntax error\n");
			free_token_list(lexed_list);
			free(command_line);
			continue ;
		}
		
		 // Print the token list and AST for debugging
		printf("\n--- Token List ---\n");
		print_token_list(lexed_list);
		printf("\n--- Abstract Syntax Tree ---\n");
		print_ast(ast, 0);
		printf("\n");
		
		// Execute the command
		execute_ast(ast, ctx);
		
		// Clean up
		free_ast(ast);*/

		free(command);
	}
	
	// This part is never reached in the current implementation
	// but good practice for memory management
}

/*
** Main function
*/
int	main(int argc, char **argv, char **envp)
{
	char	*prompt;

	(void)argc;
	(void)argv;
	
	clear_screen();
	handle_signals();
	prompt = prepare_prompt(envp);
	shell_loop(prompt, envp);
	free(prompt);
	
	return (0);
}
