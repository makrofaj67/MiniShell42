/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 23:10:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdio.h>

int	builtin_echo(char **argv)
{
	int	idx;
	int	newline;

	idx = 1;
	newline = 1;
	if (argv[1] && !strcmp(argv[1], "-n"))
	{
		newline = 0;
		idx++;
	}
	while (argv[idx])
	{
		printf("%s", argv[idx]);
		if (argv[idx + 1])
			printf(" ");
		idx++;
	}
	if (newline)
		printf("\n");
	return (0);
}
