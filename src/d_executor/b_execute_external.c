/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_execute_external.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:25:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/22 11:25:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char	*find_command_path(char *cmd, t_variable_list *env_list)
{
	char			*path_env;
	char			**paths;
	char			*cmd_path;
	int				i;
	struct stat		st;

	if (!cmd || !*cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (ft_strdup(cmd));
	path_env = get_variable_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	free(path_env); // Free the allocated path_env
	i = 0;
	while (paths && paths[i])
	{
		cmd_path = ft_strjoin3(paths[i], "/", cmd);
		if (stat(cmd_path, &st) == 0 && (st.st_mode & S_IXUSR))
			break ;
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	free_str_array(paths);
	return (cmd_path);
}

char	**create_env_array(t_variable_list *env_list)
{
	char			**env_array;
	t_variable_node	*current;
	int				i;
	char			*temp;

	if (!env_list || !env_list->head)
		return (NULL);
	current = env_list->head;
	i = 0;
	while (current)
	{
		if (current->is_exported)
			i++;
		current = current->next;
	}
	env_array = (char **)malloc(sizeof(char *) * (i + 1));
	if (!env_array)
		return (NULL);
	current = env_list->head;
	i = 0;
	while (current)
	{
		if (current->is_exported)
		{
			temp = ft_strjoin(current->key, "=");
			env_array[i] = ft_strjoin(temp, current->value);
			free(temp);
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

void	free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
