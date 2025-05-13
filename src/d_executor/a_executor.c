/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_executor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/13 18:00:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/**
 * Verilen komutu çalıştırır
 * Dahili komutları doğrudan çalıştırır, harici komutları PATH üzerinden arar
 */
void	execute_command(char **args, t_env **env_list, t_env **export_list)
{
	int	builtin_result;
	
	if (!args || !args[0])
		return;
		
	// Önce dahili komutları kontrol et
	builtin_result = builtin_selector(args, env_list, export_list);
	
	// Komut builtin değilse PATH'te ara
	if (builtin_result == -1)
	{
		// Harici komut çalıştırma
		char *cmd_path = find_command_path(args[0], *env_list);
		if (cmd_path)
		{
			execute_external_command(args, *env_list);
			free(cmd_path);
		}
		else
		{
			// Komut bulunamadı
			fprintf(stderr, "%s: command not found\n", args[0]);
		}
	}
}

/**
 * PATH çevre değişkeninden komutu arar
 * Bulunan tam yolu döndürür, bulunamazsa NULL döner
 */
char	*find_command_path(char *command, t_env *env_list)
{
	char *path_env;
	char *path_copy;
	char *path_token;
	char *cmd_path;
	
	// PATH değişkenini al
	path_env = env_value("PATH", env_list);
	if (!path_env)
		return (NULL);
	
	// Mutlak yol verilmişse doğrudan kontrol et
	if (command[0] == '/' || command[0] == '.')
	{
		if (access(command, X_OK) == 0)
			return (strdup(command));
		return (NULL);
	}
	
	// PATH çevre değişkenini parçala ve her bir dizinde komut var mı kontrol et
	path_copy = strdup(path_env);
	path_token = strtok(path_copy, ":");
	
	while (path_token)
	{
		cmd_path = mini_strcat(path_token, command);
		if (access(cmd_path, X_OK) == 0)
		{
			free(path_copy);
			return (cmd_path);
		}
		free(cmd_path);
		path_token = strtok(NULL, ":");
	}
	
	free(path_copy);
	return (NULL);
}

/**
 * fork() ile harici komutu çalıştırır
 */
void	execute_external_command(char **args, t_env *env_list)
{
	pid_t	pid;
	int		status;
	char	**envp;
	char	*cmd_path;
	
	// PATH'te komut ara
	cmd_path = find_command_path(args[0], env_list);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: command not found\n", args[0]);
		return;
	}
	
	// Çevre değişkenlerini diziye dönüştür
	// (Bu işlev gerçek uygulamada env_list'ten envp dizisi oluşturmalıdır)
	envp = environ;
	
	pid = fork();
	if (pid == 0)
	{
		// Çocuk süreç
		handle_redirection(args);
		execve(cmd_path, args, envp);
		perror("execve");
		exit(1);
	}
	else if (pid > 0)
	{
		// Ebeveyn süreç
		waitpid(pid, &status, 0);
	}
	else
	{
		// Fork hatası
		perror("fork");
	}
	
	free(cmd_path);
}

/**
 * Komut satırındaki yönlendirmeleri işler (>, <, >>, <<)
 */
void	handle_redirection(char **args)
{
	int	i;
	int	fd;
	
	i = 0;
	while (args[i])
	{
		// Çıkış yönlendirme (>)
		if (strcmp(args[i], ">") == 0 && args[i+1])
		{
			fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("open");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL; // Yönlendirmeyi komut argümanlarından kaldır
		}
		// Girdi yönlendirme (<)
		else if (strcmp(args[i], "<") == 0 && args[i+1])
		{
			fd = open(args[i+1], O_RDONLY);
			if (fd < 0)
			{
				perror("open");
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			args[i] = NULL;
		}
		// Diğer yönlendirmeler (>> ve <<) burada eklenebilir
		
		i++;
	}
}
