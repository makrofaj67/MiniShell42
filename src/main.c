/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:30:36 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 23:07:14 by rakman           ###   ########.fr       */
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
	t_token_list	*tokens;
	int             exit_status;

	should_exit = 0;
	exit_status = 0;  // Başlangıçta çıkış kodu 0
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
		free_token_list(tokens);  // Free the token list to prevent memory leaks
		free(command);
		
		// Normalde burada komutunuz çalıştırılacak ve exit_status güncellenecek
		// Şimdilik basitçe exit_status'u 0 olarak bırakalım
		// Gerçek executor implementasyonunda bu değer güncellenmeli
		// exit_status = <last command's exit status>;
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
