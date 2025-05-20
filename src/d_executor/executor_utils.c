
#include "../../inc/__minishell.h"

int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
			!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
			!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
			!ft_strcmp(cmd, "exit"));
}

int execute_builtin(char **args, t_env **env_list, t_env **export_list)
{
	if (!args || !args[0])
		return (1);

	if (!ft_strcmp(args[0], "echo"))
		return (echo_cmd(args, env_list));
	else if (!ft_strcmp(args[0], "cd"))
		return (cd_cmd(args));
	else if (!ft_strcmp(args[0], "pwd"))
	{
		pwd_cmd();
		return (0);
	}
	else if (!ft_strcmp(args[0], "export"))
	{
		if (args[1])
			export_cmd(args[1], env_list, export_list);
		else
			print_nodes(export_list); 
		return (0);
	}
	else if (!ft_strcmp(args[0], "unset"))
	{
		if (args[1])
			unset_cmd(args[1], env_list);
		return (0);
	}
	else if (!ft_strcmp(args[0], "env"))
	{
		print_nodes(env_list); 
		return (0);
	}
	else if (!ft_strcmp(args[0], "exit"))
		return (exit_cmd(args));

	return (1);
}

char **env_to_array(t_env *env_list)
{
	int		count;
	char	**env_array;
	t_env	*current;
	int		i;
	char	*tmp;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}

	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);

	i = 0;
	current = env_list;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		if (!tmp)
		{
			free_array(env_array);
			return (NULL);
		}
		env_array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		if (!env_array[i])
		{
			free_array(env_array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

char *find_command_in_path(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	path_env = NULL;
	while (env_list)
	{
		if (!ft_strcmp(env_list->key, "PATH"))
		{
			path_env = env_list->value;
			break;
		}
		env_list = env_list->next;
	}
	
	if (!path_env)
		return (NULL);

	paths = split_by_char(path_env, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		full_path = path_join(paths[i], cmd);
		if (!full_path)
		{
			free_array(paths);
			return (NULL);
		}
		
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		
		free(full_path);
		i++;
	}
	
	free_array(paths);
	return (NULL);
}

char *path_join(char *dir, char *file)
{
	char	*tmp;
	char	*result;
	int		len;

	len = ft_strlen(dir);
	
	if (len > 0 && dir[len - 1] == '/')
		return (ft_strjoin(dir, file));
	
	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	
	result = ft_strjoin(tmp, file);
	free(tmp);
	
	return (result);
}

void free_array(char **array)
{
	int	i;

	if (!array)
		return;
	
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char **split_by_char(char *str, char c)
{
	int		count;
	char	**result;
	int		i;
	int		start;
	int		j;

	count = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}

	result = (char **)malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);

	i = 0;
	j = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == c || str[i + 1] == '\0')
		{
			if (str[i] == c)
				result[j] = ft_substr(str, start, i - start);
			else
				result[j] = ft_substr(str, start, i - start + 1);
			
			if (!result[j])
			{
				free_array(result);
				return (NULL);
			}
			j++;
			start = i + 1;
		}
		i++;
	}
	result[j] = NULL;
	return (result);
}

int exit_cmd(char **args)
{
	int	exit_code;

	if (!args[1])
	{
		printf("exit\n");
		exit(0);
	}
	
	if (!is_numeric(args[1]))
	{
		printf("exit\n");
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
		exit(255);
	}
	
	if (args[1] && args[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		return (1);
	}
	
	exit_code = ft_atoi(args[1]);
	printf("exit\n");
	exit(exit_code % 256);
}

int is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	
	return (1);
}

int ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	
	return (result * sign);
}
