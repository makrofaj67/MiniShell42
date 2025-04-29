/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:30:36 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 12:15:21 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/__minishell.h"

void	shell_loop(char *prompt)
{
	char		*command_line;
	t_t_list	*lexed_list;

	while (true)
	{
		command_line = get_command(prompt);
		if (command_line == NULL)
			continue ;
		lexed_list = get_lexed(command_line);
		print_token_list(lexed_list);
		free_token_list(lexed_list);
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
