/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:51:51 by rakman            #+#    #+#             */
/*   Updated: 2025/04/28 16:24:57 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <readline/history.h>

void	shell_loop(char *prompt)
{
	char	*command_line;

	while (true)
	{
		command_line = get_command(prompt);
		if (!command_line)
			continue ;
		printf("Command: %s\n", command_line);
		free(command_line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*prompt;

	clear_screen();
	handle_signals();
	prompt = prepare_prompt(envp);
	shell_loop(prompt);
	free(prompt);
}
