/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:28:55 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 20:29:15 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void *perror_rnull(char *message)
{
	printf("%s", message);
	return (NULL);
}

void *perror_exit(char *message)
{
	printf("%s", message);
	exit(EXIT_SUCCESS);
}
