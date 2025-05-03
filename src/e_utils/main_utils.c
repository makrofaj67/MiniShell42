/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:14:55 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 21:59:33 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <curses.h>
#include <term.h>



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

