/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 00:32:21 by rakman            #+#    #+#             */
/*   Updated: 2025/04/27 01:10:18 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void	clear_screen(void)
{
	int		id;
	char	*args[2];
	char	*envp[2];

	args[0] = "/usr/bin/clear";
	args[1] = NULL;
	envp[0] = "TERM=xterm-256color";
	envp[1] = NULL;
	id = fork();
	if (id == 0)
		execve(args[0], args, envp);
	else
		waitpid(id, NULL, 0);
}

void	handle_signal(int sig)
{
	(void)sig;
	clear_screen();
	exit(0);
}

void	print_prompt(void)
{
	char	*user;

	user = getenv("USER");
	if (user == NULL)
		printf("\033[38;5;208m\n╭─┤ user\033[31m is so ");
	else
		printf("\033[38;5;208m╭─┤ %s\033[31m is so ", user);
	printf("\033[38;5;208mKoo⅃\033[31m in "
		"\033[38;5;208mMiniHeLL |\n");
}

int	main(int argc, char **argv, char **envp)
{
	char				*command;
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGKILL, &sa, NULL);
	clear_screen();
	while (1)
	{
		print_prompt();
		command = readline("\033[38;5;208m╰─λ \033[0m");
		if (command == NULL) //ctrl D
			exit(EXIT_SUCCESS);
		if (strlen(command) == 0 || is_all_whitespace(command))
		{
			free(command);
			continue;
		}
		add_history(command);
		if (command_lexer(command) != 0)
		{
			free(command);
			continue;
		}
		if (command_parser(command) != 0)
		{
			free(command);
			continue;
		}
		execute_command(command);
		free(command);
	}
	return (0);
}
