/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 00:32:21 by rakman            #+#    #+#             */
/*   Updated: 2025/04/27 01:08:18 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void	clear_screen(void)
{
	int		id;
	char	*term_value;
	char	term_env[100];
	char	*args[2];
	char	*envp[2];

	term_value = getenv("TERM");
	if (term_value)
	{
		strcpy(term_env, "TERM=");
		strcat(term_env, term_value);
		envp[0] = term_env;
	}
	else
		envp[0] = "TERM=xterm";
	envp[1] = NULL;
	args[0] = "/usr/bin/clear";
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
		printf("\033[38;5;208m╭─┤ user\033[31m is so ");
	else
		printf("\033[38;5;208m╭─┤ %s\033[31m is so ", user);
	printf("\033[38;5;208mKoo⅃\033[31m in "
		"\033[38;5;208mMiniHeLL |\n"
		"\033[38;5;208m╰─λ \033[0m");
}

int	main(int argc, char **argv, char **envp)
{
	int		status;
	char	*command;

	(void)argc;
	(void)argv;
	(void)envp;
	status = 1;
	signal(SIGINT, handle_signal);
	clear_screen();
	while (status == 1)
	{
		print_prompt();
		command = readline(NULL);
		free(command);
	}
	return (0);
}
