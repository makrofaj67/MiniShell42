/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:05:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/22 12:05:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	str_to_exit_code(char *str)
{
	long	num;
	int		sign;
	int		i;

	num = 0;
	sign = 1;
	i = 0;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	num *= sign;
	return ((num % 256 + 256) % 256);
}

int	ft_exit(char **args, int *exit_status)
{
	int	status;

	printf("exit\n");
	if (!args || !args[0])
	{
		exit(*exit_status);
		return (0);
	}
	if (!is_numeric(args[0]))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			args[0]);
		exit(255);
	}
	status = str_to_exit_code(args[0]);
	if (args[1])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		*exit_status = 1;
		return (1);
	}
	exit(status);
	return (0);
}
