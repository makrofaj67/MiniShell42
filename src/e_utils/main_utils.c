/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:14:55 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 23:26:32 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void	clear_screen(void)
{
	char	*value;
	char	*term_var;
	char	*args[2];
	char	*envp[2];
	int		id;

	value = getenv("TERM");
	if (!value)
		return ;
	term_var = (char *)malloc(sizeof(char)
			* (strlen("TERM=") + strlen(value) + 1));
	if (!term_var)
		return ;
	strcpy(term_var, "TERM=");
	strcat(term_var, value);
	envp[0] = term_var;
	envp[1] = NULL;
	args[0] = "/usr/bin/clear";
	args[1] = NULL;
	id = fork();
	if (id == 0)
		execve(args[0], args, envp);
	else if (id > 0)
		waitpid(id, NULL, 0);
	free(term_var);
}

char	*prepare_prompt(char **envp)
{
	char	*user;
	char	*all;
	size_t	len;

	user = getenv("USER");
	if (user == NULL)
		user = "unknown";
	len = strlen("\033[38;5;208m╭─┤ ") + strlen(user)
		+ strlen("\033[31m is so \033[38;5;208mKoo⅃\033[31m in ")
		+ strlen("\033[38;5;208mMiniHeLL |\n\033[38;5;208m╰─λ \033[0m") + 1;
	all = (char *)malloc(len);
	if (all == NULL)
		return (NULL);
	all[0] = '\0';
	strcat(all, "\033[38;5;208m╭─┤ ");
	strcat(all, user);
	strcat(all, "\033[31m is so \033[38;5;208mKoo⅃\033[31m in ");
	strcat(all, "\033[38;5;208mMiniHeLL |\n\033[38;5;208m╰─λ \033[0m");
	return (all);
}
