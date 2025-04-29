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
	char			*command_line;
	t_t_list		*lexed_list;
	t_ast_node		*ast;
	t_exec_context	*ctx;

	ctx = init_exec_context(envp);
	if (!ctx)
		return ;
	while (true)
	{
		command_line = get_command(prompt);
		if (command_line == NULL)
			continue ;
		
		// Lexical analysis - convert command to tokens
		lexed_list = get_lexed(command_line);
		if (!lexed_list)
		{
			free(command_line);
			continue ;
		}
		
		print_token_list(lexed_list);
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

		free_token_list(lexed_list);
		free(command_line);
	}
	
	// This part is never reached in the current implementation
	// but good practice for memory management
	free(ctx);
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
