#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>

char	**split_line(char *line);
void	free_tokens(char **tokens);

char	*find_command_in_path(const char *cmd);
char	*search_in_path(char *path_copy, const char *cmd);
char	*create_full_path(const char *dir, const char *cmd);

int		is_builtin_command(char *command);
int		execute_builtin_command(char **args, int *shell_active);
int		execute_cd_command(char **args);
int		execute_external_command(char *command_path, char **args,
			char **env);

void	process_non_empty_line(char *line, char **env, int *shell_active);

char	**split_line(char *line)
{
	int		bufsize;
	char	**tokens;
	int		position;
	char	*token;
	char	**new_tokens;

	if (line == NULL)
		return (NULL);
	bufsize = 64;
	tokens = malloc(bufsize * sizeof(char *));
	if (!tokens)
	{
		perror("split_line");
		return (NULL);
	}
	position = 0;
	token = strtok(line, " \t\n");
	if (token == NULL)
	{
		tokens[0] = NULL;
		return (tokens);
	}
	while (token != NULL)
	{
		tokens[position] = token;
		position++;
		if (position >= bufsize)
		{
			bufsize += 64;
			new_tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!new_tokens)
			{
				perror("split_line");
				free(tokens);
				return (NULL);
			}
			tokens = new_tokens;
		}
		token = strtok(NULL, " \t\n");
	}
	tokens[position] = NULL;
	return (tokens);
}

void	free_tokens(char **tokens)
{
	if (tokens)
		free(tokens);
}

char	*find_command_in_path(const char *cmd)
{
	char	*path_env;
	char	*path_copy;

	if (cmd == NULL || *cmd == '\0')
		return (NULL);
	if (strchr(cmd, '/') != NULL)
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}
	path_env = getenv("PATH");
	if (path_env == NULL || *path_env == '\0')
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}
	path_copy = strdup(path_env);
	if (path_copy == NULL)
	{
		perror("find_command_in_path");
		return (NULL);
	}
	return (search_in_path(path_copy, cmd));
}

char	*search_in_path(char *path_copy, const char *cmd)
{
	char	*dir;
	char	*full_path;

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		full_path = create_full_path(dir, cmd);
		if (full_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		full_path = NULL;
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

char	*create_full_path(const char *dir, const char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;
	size_t	fullpath_len;
	char	*full_path;

	dir_len = strlen(dir);
	cmd_len = strlen(cmd);
	fullpath_len = dir_len + 1 + cmd_len + 1;
	full_path = malloc(fullpath_len);
	if (full_path == NULL)
	{
		perror("create_full_path");
		return (NULL);
	}
	strcpy(full_path, dir);
	strcat(full_path, "/");
	strcat(full_path, cmd);
	return (full_path);
}

int	is_builtin_command(char *command)
{
	if (command == NULL)
		return (0);
	if (strcmp(command, "exit") == 0)
		return (1);
	if (strcmp(command, "cd") == 0)
		return (1);
	return (0);
}

int	execute_builtin_command(char **args, int *shell_active)
{
	if (args == NULL || args[0] == NULL)
		return (1);
	if (strcmp(args[0], "exit") == 0)
	{
		printf("Shell'den çikiliyor.\n");
		*shell_active = 0;
		return (0);
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		return (execute_cd_command(args));
	}
	return (1);
}

int	execute_cd_command(char **args)
{
	char	*dir_path;

	if (args[1] == NULL || strcmp(args[1], "~") == 0)
	{
		dir_path = getenv("HOME");
		if (dir_path == NULL)
		{
			fprintf(stderr, "HALFXLIFE: cd: HOME not set\n");
			return (1);
		}
	}
	else
		dir_path = args[1];
	if (chdir(dir_path) != 0)
	{
		perror("HALFXLIFE: cd");
		return (1);
	}
	return (0);
}

int	execute_external_command(char *command_path, char **args, char **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("shell: fork error");
		return (1);
	}
	if (pid == 0)
	{
		execve(command_path, args, env);
		perror("shell: execve error");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	return (1);
}

void	process_non_empty_line(char *line, char **env, int *shell_active)
{
	char	**args;
	char	*command_path;

	add_history(line);
	args = split_line(line);
	if (args == NULL || args[0] == NULL)
	{
		if (args != NULL)
			free_tokens(args);
	}
	else if (is_builtin_command(args[0]))
	{
		execute_builtin_command(args, shell_active);
		free_tokens(args);
	}
	else
	{
		command_path = find_command_in_path(args[0]);
		if (command_path != NULL)
		{
			execute_external_command(command_path, args, env);
			free(command_path);
		}
		else
			fprintf(stderr, "HALFXLIFE: command not found: %s\n", args[0]);
		free_tokens(args);
	}
	free(line);
}

int	main(int argc, char **argv, char **env)
{
	int		shell_active;
	char	*line;

	(void)argc;
	(void)argv;
	shell_active = 1;
	while (shell_active)
	{
		line = readline("HALFXLIFE>> ");
		if (line == NULL)
		{
			printf("exit\n");
			shell_active = 0;
		}
		else if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		else
			process_non_empty_line(line, env, &shell_active);
	}
	return (0);
}