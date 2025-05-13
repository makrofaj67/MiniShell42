/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selector.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:17:54 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 17:45:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int	builtin_selector(char **args, t_env **env_list, t_env **export_list)
{
	if (!args || !args[0])
		return (0);
		
	if (mini_strcmp_path(args[0], "pwd") == 1)
		return pwd_cmd();
	else if (mini_strcmp_path(args[0], "clear") == 1)
	{
		printf("\033[H\033[2J");
		return (0);
	}
	else if (mini_strcmp_path(args[0], "exit") == 1)
		return (1); // Çıkış sinyali
	else if (mini_strcmp_path(args[0], "cd") == 1)
		return cd_cmd(args, env_list);
	else if (mini_strcmp_path(args[0], "echo") == 1)
		return echo_cmd(args);
	else if (mini_strcmp_path(args[0], "unset") == 1)
	{
		if (args[1])
			return unset_cmd(args[1], env_list);
		else
			return (0);
	}
	else if (mini_strcmp_path(args[0], "env") == 1)
	{
		print_nodes(*env_list);
		return (0);
	}
	else if (mini_strcmp_path(args[0], "export") == 1)
	{
		if (args[1])
			return export_cmd(args[1], env_list, export_list);
		else
		{
			print_nodes(*export_list);
			return (0);
		}
	}
	else if (is_variable(args[0]))
	{
		add_list_variable(args, export_list);
		return (0);
	}
	
	return (-1); // Dahili komut değil
}
