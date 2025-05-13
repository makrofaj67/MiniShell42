/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:54 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 17:40:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int	pwd_cmd(void)
{
	char	pwd[1024];
	
	if (getcwd(pwd, sizeof(pwd)) == NULL)
	{
		perror("pwd: error retrieving current directory");
		return (1);
	}
	
	printf("%s\n", pwd);
	return (0);
}
