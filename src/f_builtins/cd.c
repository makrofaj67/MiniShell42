/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:01:07 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 17:20:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char *get_path_home(void)
{
	char *path = getenv("HOME");
	if (path == NULL)
	{
		write(2, "Error: HOME environment variable not set.\n", 42);
		return NULL;
	}
	return path;
}

int	cd_cmd(char **args, t_env **env_list)
{
	char old_pwd[1024];
	
	// Mevcut çalışma dizinini kaydet
	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
	{
		write(2, "cd: error retrieving current directory\n", 39);
		return (1);
	}
	
	// Dizin değiştir
	if (!args[1])
	{
		if (chdir(get_path_home()) == -1)
		{
			write(2, "cd: HOME directory error\n", 25);
			return (1);
		}
	}
	else if (chdir(args[1]) == -1)
	{
		write(2, "cd: directory not found\n", 24);
		return (1);
	}
	
	// OLDPWD ve PWD değişkenlerini güncelle
	char current_pwd[1024];
	if (getcwd(current_pwd, sizeof(current_pwd)) != NULL)
	{
		// Çevre değişkenlerini güncelle
		// (Gerçek uygulama burada env_list'i güncellemelidir)
	}
	
	return (0);
}
