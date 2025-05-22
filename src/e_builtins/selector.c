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

#include "../../inc/__minishell.h"

void	print_nodes_var(t_env **tmp)
{
	t_env *env;
    if (tmp == NULL || *tmp == NULL)
    {
        printf("Environment list is empty.\n");
        return;
    }
	env = *tmp;
	while (env != NULL)
	{
		printf("declare -x %s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int		builtin_selector(char **args, t_env **env, t_env **env_var)
{
	if (mini_strcmp_path(args[0], "pwd") == 1)
		pwd_cmd();
	else if (mini_strcmp_path(args[0], "clear") == 1)
		printf("\033[H\033[2J");
	else if (mini_strcmp_path(args[0], "exit"))
		exit(EXIT_SUCCESS);
	else if (mini_strcmp_path(args[0], "cd"))
		cd_cmd(args);
	else if (mini_strcmp_path(args[0], "echo"))
		echo_cmd(args, env_var); 
	else if (mini_strcmp_path(args[0], "unset"))
		unset_cmd(args[1], env);
	else if (mini_strcmp_path(args[0], "env"))
		print_nodes(env);
	else if (mini_strcmp_path(args[0], "export"))
		if (args[1])
			export_cmd(args[1], env, env_var);
		else
			print_nodes_var(env_var);
	else if (is_variable(args))
		add_list_variable(args, env_var);
	else
		return (1);
	return (0);
}
