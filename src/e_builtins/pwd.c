/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:54 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 12:00:54 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void mini_memset(char *ptr, int size)
{
    int i = 0;
    while (i < size)
    {
        ptr[i] = '\0';
        i++;
    }
}

void pwd_cmd()
{
    char pwd[256];
    
    if (getcwd(pwd, sizeof(pwd)) != NULL)
        printf("%s\n", pwd);
    else
        perror("getcwd failed");
    
    mini_memset(pwd, sizeof(pwd));
}
