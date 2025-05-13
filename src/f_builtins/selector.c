/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selector.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:17:54 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 12:17:54 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/f_builtins.h"

int		builtin_selector(char **args, t_env **env, t_env **env_var)
{
	if (!args || !args[0])
		return (1);

	if (mini_strcmp_path(args[0], "pwd") == 1)
		pwd_cmd();
	else if (mini_strcmp_path(args[0], "clear") == 1)
		printf("\033[H\033[2J");
	else if (mini_strcmp_path(args[0], "exit") == 1)
		exit(EXIT_SUCCESS);
	else if (mini_strcmp_path(args[0], "cd") == 1)
		cd_cmd(args);
	else if (mini_strcmp_path(args[0], "echo") == 1)
		echo_cmd(args, env_var);
	else if (mini_strcmp_path(args[0], "unset") == 1)
	{
		if (args[1])
			unset_cmd(args[1], env);
	}
	else if (mini_strcmp_path(args[0], "env") == 1)
		print_nodes(env);
	else if (mini_strcmp_path(args[0], "export") == 1)
	{
		if (args[1])
			export_cmd(args[1], env, env_var);
	}
	else if (is_variable(args))
		add_list_variable(args, env_var);
	else
		return (1);
	return (0);
}
