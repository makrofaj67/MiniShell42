/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:15:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/20 03:15:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Check if a string is the '-n' option
** Must be exactly "-n" or "-nnn..." (any number of 'n's)
*/
static int is_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	
	i = 1;
	while (arg[i] && arg[i] == 'n')
		i++;
	
	return (arg[i] == '\0');  // True if we reached the end of the string
}

/*
** Echo built-in command implementation
** Supports -n option to suppress trailing newline
*/
int echo_cmd(char **args, t_env **env_list)
{
	int	i;
	int	n_option;

	n_option = 0;
	i = 1;

	// Check for -n option
	while (args[i] && is_n_option(args[i]))
	{
		n_option = 1;
		i++;
	}

	// Print arguments
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}

	// Print newline if -n option was not specified
	if (!n_option)
		printf("\n");
	
	return (0);
}
