/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:01:07 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 12:01:07 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char *get_path_home(void)
{
	char *path = getenv("HOME");
	if (path == NULL)
	{
		write(2, "Error: PATH environment variable not set.\n", 42);
		return NULL;
	}
	return path;
}

void	cd_cmd(char **args)
{	
	if (!args[1])
		chdir(get_path_home());
	else if (chdir(args[1]) == -1)
		write(2, "cd error\n", 9);
}
