/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:28:22 by rakman            #+#    #+#             */
/*   Updated: 2025/04/27 18:41:55 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*prompt;
	char	*command;

	clear_screen();
	handle_signals();
	prompt = prepare_prompt(envp);
	while (1)
	{
		command = readline(prompt);
		free(command);
	}
	free(prompt);
}
