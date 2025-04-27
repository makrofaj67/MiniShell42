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

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void clear_screen(void)
{
    int id;
    char *args[2];
    char *envp[2];

    args[0] = "/usr/bin/clear";
    args[1] = NULL;  // This was missing - args array needs to be NULL-terminated
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
		printf("\033[38;5;208m╭─┤ user\033[31m is so ");
	else
		printf("\033[38;5;208m╭─┤ %s\033[31m is so ", user);
	printf("\033[38;5;208mKoo⅃\033[31m in "
		"\033[38;5;208mMiniHeLL |\n"
		"\033[38;5;208m╰─λ \033[0m");
}

int main(int argc, char **argv, char **envp)
{
    int status;
    char *command;

    (void)argc;
    (void)argv;
    (void)envp;
    status = 1;
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);  // For Ctrl+C
    clear_screen();
    while (status == 1)
    {
        print_prompt();
        command = readline(NULL);
        free(command);
    }
    return (0);
}

