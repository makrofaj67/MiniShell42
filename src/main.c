/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:30:36 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 21:09:15 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/__minishell.h"
#include <unistd.h>

/*
** The main shell loop
*/
void	shell_loop(char *prompt, char **envp)
{
	char			*command;
	int				should_exit;

	should_exit = 0;
	while (true)
	{
		command = get_command(prompt, &should_exit);
		if (command == NULL)
		{
			if (should_exit == 1)
				exit(EXIT_SUCCESS);
			continue ;
		}
		printf("-->%s<--\n", command);
		free(command);
	}
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
	setup_interactive_signals();
	prompt = prepare_prompt(envp);
	shell_loop(prompt, envp);
	free(prompt);
	rl_clear_history();
	return (0);
}
