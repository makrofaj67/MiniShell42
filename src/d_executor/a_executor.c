/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_executor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 21:25:18 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 21:49:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include "../../inc/d_executor.h"
#include "../../inc/f_builtins.h"
#include <errno.h>

/**
 * Initialize execution context for command execution
 */
t_exec_context *init_exec_context(char **envp)
{
	t_exec_context *context;

	context = (t_exec_context *)malloc(sizeof(t_exec_context));
	if (!context)
		return (NULL);
	context->envp = envp;
	context->prev_pipe[0] = -1;
	context->prev_pipe[1] = -1;
	context->curr_pipe[0] = -1;
	context->curr_pipe[1] = -1;
	context->exit_status = 0;
	context->is_first_cmd = 1;
	context->is_last_cmd = 1;
	context->pipe_count = 0;
	context->pids = (pid_t *)malloc(sizeof(pid_t) * MAX_PIPES);
	if (!context->pids)
	{
		free(context);
		return (NULL);
	}
	context->heredoc_count = 0;
	return (context);
}

/**
 * Free the execution context and all associated resources
 */
void free_exec_context(t_exec_context *context)
{
	if (!context)
		return;
	if (context->pids)
		free(context->pids);
	if (context->prev_pipe[0] != -1)
		close(context->prev_pipe[0]);
	if (context->prev_pipe[1] != -1)
		close(context->prev_pipe[1]);
	if (context->curr_pipe[0] != -1)
		close(context->curr_pipe[0]);
	if (context->curr_pipe[1] != -1)
		close(context->curr_pipe[1]);
	free(context);
}

/**
 * Find executable path in the PATH environment variable
 */
char *find_executable(char *cmd, char **envp)
{
	char *path_env;
	char *path;
	char *token;
	char *exec_path;
	int i;

	if (!cmd || !*cmd)
		return (NULL);
	// If command contains a '/', treat as a path
	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}
	// Find PATH in environment
	i = 0;
	path_env = NULL;
	while (envp[i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break;
		}
		i++;
	}
	if (!path_env)
		return (NULL);
	// Copy PATH to avoid modifying the original
	path = strdup(path_env);
	if (!path)
		return (NULL);
	// Tokenize PATH and check each directory
	token = strtok(path, ":");
	while (token)
	{
		exec_path = (char *)malloc(strlen(token) + strlen(cmd) + 2);
		if (!exec_path)
		{
			free(path);
			return (NULL);
		}
		strcpy(exec_path, token);
		strcat(exec_path, "/");
		strcat(exec_path, cmd);
		if (access(exec_path, X_OK) == 0)
		{
			free(path);
			return (exec_path);
		}
		free(exec_path);
		token = strtok(NULL, ":");
	}
	free(path);
	return (NULL);
}

/**
 * Handle a heredoc redirection
 */
char *handle_heredoc(char *delimiter, t_exec_context *context)
{
	char *tmp_filename;
	char *line;
	int fd;
	char *heredoc_num_str;

	// Create unique temporary filename
	heredoc_num_str = (char *)malloc(12);
	if (!heredoc_num_str)
		return (NULL);
	sprintf(heredoc_num_str, "%d", context->heredoc_count++);
	tmp_filename = ft_strjoin("/tmp/heredoc_", heredoc_num_str);
	free(heredoc_num_str);
	if (!tmp_filename)
		return (NULL);
	// Open temporary file
	fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(tmp_filename);
		return (NULL);
	}
	// Read lines until delimiter is found
	while (1)
	{
		// Use readline for interactive input
		line = readline("> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (tmp_filename);
}

/**
 * Apply input/output redirections for a command
 */
int apply_redirections(t_redirection **redirections, t_exec_context *context)
{
	int i;
	int fd;
	char *heredoc_file;

	if (!redirections)
		return (0);
	i = 0;
	while (redirections[i])
	{
		switch (redirections[i]->type)
		{
		case REDIR_IN:
			fd = open(redirections[i]->filename, O_RDONLY);
			if (fd < 0)
			{
				perror(redirections[i]->filename);
				return (-1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			break;
		case REDIR_OUT:
			fd = open(redirections[i]->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror(redirections[i]->filename);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			break;
		case REDIR_APPEND:
			fd = open(redirections[i]->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror(redirections[i]->filename);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			break;
		case REDIR_HEREDOC:
			heredoc_file = handle_heredoc(redirections[i]->filename, context);
			if (!heredoc_file)
				return (-1);
			fd = open(heredoc_file, O_RDONLY);
			if (fd < 0)
			{
				perror(heredoc_file);
				free(heredoc_file);
				return (-1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			free(heredoc_file);
			break;
		}
		i++;
	}
	return (0);
}

/**
 * Execute a simple command with arguments and handle builtin commands
 */
int execute_simple_command(ast_node *cmd_node, t_exec_context *context)
{
	pid_t pid;
	char *cmd_path;
	int status;
	command_value *cmd;

	if (!cmd_node || cmd_node->type != COMMAND_NODE || !cmd_node->value)
		return (1);
	
	cmd = (command_value *)cmd_node->value;
	if (!cmd->arg_array || !cmd->arg_array[0])
		return (0);  // Empty command, nothing to do

	// Check if this is a builtin command
	if (is_builtin(cmd->arg_array[0]))
	{
		// Apply redirections for builtin
		int stdin_backup = dup(STDIN_FILENO);
		int stdout_backup = dup(STDOUT_FILENO);
		
		// Set up pipes if in a pipeline
		if (!context->is_first_cmd)
			dup2(context->prev_pipe[0], STDIN_FILENO);
		if (!context->is_last_cmd)
			dup2(context->curr_pipe[1], STDOUT_FILENO);
		
		// Apply any redirections
		if (apply_redirections(cmd->redirections, context) < 0)
		{
			dup2(stdin_backup, STDIN_FILENO);
			dup2(stdout_backup, STDOUT_FILENO);
			close(stdin_backup);
			close(stdout_backup);
			return (1);
		}
		
		// Execute the builtin command
		status = execute_builtin(cmd, context);
		
		// Restore standard I/O
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		
		// Close any pipes and prepare for next command if needed
		if (!context->is_last_cmd)
		{
			close(context->curr_pipe[1]);  // Close write end of current pipe
			if (context->prev_pipe[0] != -1)
				close(context->prev_pipe[0]);  // Close previous read pipe if exists
			context->prev_pipe[0] = context->curr_pipe[0];  // Save read end for next command
			context->prev_pipe[1] = -1;
		}
		
		return (status);
	}

	// For non-builtin commands, fork and exec
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		// Child process
		// Set up pipes if in a pipeline
		if (!context->is_first_cmd)
		{
			dup2(context->prev_pipe[0], STDIN_FILENO);
		}
		if (!context->is_last_cmd)
		{
			dup2(context->curr_pipe[1], STDOUT_FILENO);
		}
		
		// Close all pipe fds in child process
		if (context->prev_pipe[0] != -1)
			close(context->prev_pipe[0]);
		if (context->prev_pipe[1] != -1)
			close(context->prev_pipe[1]);
		if (context->curr_pipe[0] != -1)
			close(context->curr_pipe[0]);
		if (context->curr_pipe[1] != -1)
			close(context->curr_pipe[1]);
		
		// Apply any redirections
		if (apply_redirections(cmd->redirections, context) < 0)
			exit(1);
		
		// Find and execute the command
		cmd_path = find_executable(cmd->arg_array[0], context->envp);
		if (!cmd_path)
		{
			fprintf(stderr, "minishell: command not found: %s\n", cmd->arg_array[0]);
			exit(127);  // Command not found exit code
		}
		
		// Execute the command
		execve(cmd_path, cmd->arg_array, context->envp);
		perror("execve");
		free(cmd_path);
		exit(126);  // Could not execute
	}
	else
	{
		// Parent process
		// Store the pid for later
		context->pids[context->pipe_count] = pid;
		
		// If we're not the last command in a pipeline, prepare for next command
		if (!context->is_last_cmd)
		{
			close(context->curr_pipe[1]);  // Close write end of current pipe
			if (context->prev_pipe[0] != -1)
				close(context->prev_pipe[0]);  // Close previous read pipe if exists
			context->prev_pipe[0] = context->curr_pipe[0];  // Save read end for next command
			context->prev_pipe[1] = -1;
		}
		
		// If this is the only command (no pipeline), wait for it
		if (context->is_first_cmd && context->is_last_cmd)
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				return (WEXITSTATUS(status));
			return (1);  // Default error code if not normal exit
		}
	}
	
	return (0);
}

/**
 * Execute a pipeline of commands
 */
int execute_pipeline(ast_node *pipe_node, t_exec_context *context)
{
	// Check if this is a valid pipe node
	if (!pipe_node || pipe_node->type != PIPE_NODE)
		return (1);
	
	// Create pipe for this command
	if (!context->is_last_cmd)
	{
		if (pipe(context->curr_pipe) < 0)
		{
			perror("pipe");
			return (1);
		}
	}
	
	// Execute left side of pipe (might be another pipe or a command)
	if (pipe_node->left)
	{
		context->is_last_cmd = 0;  // Not the last if there's a right side
		if (pipe_node->left->type == PIPE_NODE)
			execute_pipeline(pipe_node->left, context);
		else
			execute_simple_command(pipe_node->left, context);
	}
	
	// Move to next command in pipeline
	context->is_first_cmd = 0;
	context->pipe_count++;
	
	// Execute right side of pipe (might be another pipe or a command)
	if (pipe_node->right)
	{
		// If this is the final command in the pipeline
		if (pipe_node->right->type == COMMAND_NODE ||
			(pipe_node->right->type == PIPE_NODE && !pipe_node->right->right))
			context->is_last_cmd = 1;
		
		if (pipe_node->right->type == PIPE_NODE)
			execute_pipeline(pipe_node->right, context);
		else
			execute_simple_command(pipe_node->right, context);
	}
	
	return (0);
}

/**
 * Wait for all child processes to complete
 */
int wait_for_processes(t_exec_context *context)
{
	int i;
	int status;
	int last_status;

	last_status = 0;
	for (i = 0; i <= context->pipe_count; i++)
	{
		if (context->pids[i] > 0)
		{
			waitpid(context->pids[i], &status, 0);
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
	}
	
	return (last_status);
}

/**
 * Main executor function - executes commands from the AST
 */
int execute_command(ast_node *root_node, int *exit_status, char **envp)
{
	t_exec_context *context;
	int result;

	if (!root_node)
		return (0);
	
	// Initialize execution context
	context = init_exec_context(envp);
	if (!context)
		return (1);
	
	// Execute the command based on node type
	if (root_node->type == COMMAND_NODE)
	{
		execute_simple_command(root_node, context);
	}
	else if (root_node->type == PIPE_NODE)
	{
		execute_pipeline(root_node, context);
	}
	
	// Wait for all processes to complete
	result = wait_for_processes(context);
	*exit_status = result;
	
	// Clean up
	free_exec_context(context);
	
	return (result);
}