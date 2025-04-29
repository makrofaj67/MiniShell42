/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:25:30 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 13:54:14 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

/*
** Expand environment variables in a string (&VAR -> value)
*/
char	*expand_env_vars(char *arg, char **env)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	int		j;
	int		k;

	if (!arg)
		return (NULL);
	result = malloc(strlen(arg) * 4 + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (arg[i])
	{
		if (arg[i] == '&' && arg[i + 1] && arg[i + 1] != ' ')
		{
			i++;
			k = 0;
			var_name = malloc(strlen(arg) + 1);
			if (!var_name)
			{
				free(result);
				return (NULL);
			}
			while (arg[i] && (isalnum(arg[i]) || arg[i] == '_'))
				var_name[k++] = arg[i++];
			var_name[k] = '\0';
			var_value = get_env_var(var_name, env);
			free(var_name);
			if (var_value)
			{
				k = 0;
				while (var_value[k])
					result[j++] = var_value[k++];
			}
		}
		else
			result[j++] = arg[i++];
	}
	result[j] = '\0';
	return (result);
}

/*
** Get the value of an environment variable
*/
char	*get_env_var(char *name, char **env)
{
	int		i;
	int		len;

	if (!name || !env)
		return (NULL);
	len = strlen(name);
	i = 0;
	while (env[i])
	{
		if (strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

/*
** Set up redirections for a command
*/
int	setup_redirections(t_redir *redirs)
{
	int	fd;

	while (redirs)
	{
		if (redirs->type == RDRT_IN)
		{
			fd = open(redirs->file, O_RDONLY);
			if (fd < 0)
				return (0);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redirs->type == RDRT_OUT)
		{
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				return (0);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redirs->type == APPEND)
		{
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				return (0);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redirs->type == HEREDOC)
		{
			// TODO: Implement HEREDOC
			// For now, this is a placeholder
			printf("HEREDOC not yet implemented\n");
		}
		redirs = redirs->next;
	}
	return (1);
}

/*
** Restore standard file descriptors after redirection
*/
int	restore_std_fds(int stdin_copy, int stdout_copy)
{
	if (stdin_copy >= 0)
	{
		dup2(stdin_copy, STDIN_FILENO);
		close(stdin_copy);
	}
	if (stdout_copy >= 0)
	{
		dup2(stdout_copy, STDOUT_FILENO);
		close(stdout_copy);
	}
	return (1);
}

/*
** Execute a simple command
*/
int	execute_command(t_ast_node *ast, t_exec_context *ctx)
{
	pid_t	pid;
	int		status;
	char	*path;
	int		i;

	if (!ast || !ast->data.cmd.args || !ast->data.cmd.args[0])
		return (0);
	
	// Save original stdin/stdout
	int stdin_copy = dup(STDIN_FILENO);
	int stdout_copy = dup(STDOUT_FILENO);
	
	// Set up redirections
	if (!setup_redirections(ast->data.cmd.redirs))
	{
		restore_std_fds(stdin_copy, stdout_copy);
		return (1);
	}

	// Process environment variables expansion
	char **expanded_args = NULL;
	i = 0;
	while (ast->data.cmd.args[i])
		i++;
	expanded_args = malloc(sizeof(char *) * (i + 1));
	if (!expanded_args)
		return (1);
	
	i = 0;
	while (ast->data.cmd.args[i])
	{
		expanded_args[i] = expand_env_vars(ast->data.cmd.args[i], ctx->env);
		i++;
	}
	expanded_args[i] = NULL;

	// TODO: Implement builtin commands
	// For now, execute all as external commands

	pid = fork();
	if (pid == 0)
	{
		// Child process
		execvp(expanded_args[0], expanded_args);
		// If execvp returns, there was an error
		printf("minishell: %s: command not found\n", expanded_args[0]);
		exit(127);
	}
	else if (pid > 0)
	{
		// Parent process
		ctx->last_pid = pid;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			ctx->last_exit_status = WEXITSTATUS(status);
		else
			ctx->last_exit_status = 1;
	}
	else
	{
		// Fork error
		printf("minishell: fork error\n");
		ctx->last_exit_status = 1;
	}

	// Free expanded arguments
	i = 0;
	while (expanded_args[i])
	{
		free(expanded_args[i]);
		i++;
	}
	free(expanded_args);

	// Restore original stdin/stdout
	restore_std_fds(stdin_copy, stdout_copy);

	return (ctx->last_exit_status);
}

/*
** Execute a pipe chain
*/
int	execute_pipe(t_ast_node *ast, t_exec_context *ctx)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (!ast || !ast->data.op.left || !ast->data.op.right)
		return (1);

	if (pipe(pipe_fd) < 0)
	{
		printf("minishell: pipe error\n");
		return (1);
	}

	pid = fork();
	if (pid == 0)
	{
		// Left command (writes to pipe)
		close(pipe_fd[READ_END]);
		dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
		close(pipe_fd[WRITE_END]);
		
		execute_ast(ast->data.op.left, ctx);
		exit(ctx->last_exit_status);
	}
	else if (pid > 0)
	{
		// Right command (reads from pipe)
		close(pipe_fd[WRITE_END]);
		
		pid = fork();
		if (pid == 0)
		{
			dup2(pipe_fd[READ_END], STDIN_FILENO);
			close(pipe_fd[READ_END]);
			
			execute_ast(ast->data.op.right, ctx);
			exit(ctx->last_exit_status);
		}
		else if (pid > 0)
		{
			close(pipe_fd[READ_END]);
			waitpid(pid, &status, 0);
			
			if (WIFEXITED(status))
				ctx->last_exit_status = WEXITSTATUS(status);
			else
				ctx->last_exit_status = 1;
			
			ctx->last_pid = pid;
		}
		else
		{
			printf("minishell: fork error\n");
			close(pipe_fd[READ_END]);
			return (1);
		}
	}
	else
	{
		printf("minishell: fork error\n");
		close(pipe_fd[READ_END]);
		close(pipe_fd[WRITE_END]);
		return (1);
	}

	return (ctx->last_exit_status);
}

/*
** Execute an AND (&&) operation
*/
int	execute_and(t_ast_node *ast, t_exec_context *ctx)
{
	if (!ast || !ast->data.op.left || !ast->data.op.right)
		return (1);

	// Execute left side
	if (execute_ast(ast->data.op.left, ctx) != 0)
		return (ctx->last_exit_status);  // Short circuit if left fails

	// Execute right side only if left succeeded
	return (execute_ast(ast->data.op.right, ctx));
}

/*
** Execute an OR (||) operation
*/
int	execute_or(t_ast_node *ast, t_exec_context *ctx)
{
	if (!ast || !ast->data.op.left || !ast->data.op.right)
		return (1);

	// Execute left side
	if (execute_ast(ast->data.op.left, ctx) == 0)
		return (0);  // Short circuit if left succeeds

	// Execute right side only if left failed
	return (execute_ast(ast->data.op.right, ctx));
}

/*
** Execute a parenthesized group
*/
int	execute_group(t_ast_node *ast, t_exec_context *ctx)
{
	if (!ast || !ast->data.group)
		return (1);

	return (execute_ast(ast->data.group, ctx));
}

/*
** Main execution function - Execute the AST
*/
int	execute_ast(t_ast_node *ast, t_exec_context *ctx)
{
	if (!ast)
		return (0);

	switch (ast->type)
	{
		case AST_COMMAND:
			return (execute_command(ast, ctx));
		case AST_PIPE:
			return (execute_pipe(ast, ctx));
		case AST_AND:
			return (execute_and(ast, ctx));
		case AST_OR:
			return (execute_or(ast, ctx));
		case AST_GROUP:
			return (execute_group(ast, ctx));
		default:
			printf("minishell: unknown AST node type\n");
			return (1);
	}
}

/*
** Initialize the execution context
*/
t_exec_context	*init_exec_context(char **env)
{
	t_exec_context	*ctx;

	ctx = malloc(sizeof(t_exec_context));
	if (!ctx)
		return (NULL);
	
	ctx->env = env;  // For now, just use the environment as-is
	ctx->last_exit_status = 0;
	ctx->last_pid = 0;
	
	return (ctx);
}

/*
** Copy the environment variables
*/
char	**copy_env(char **env)
{
	char	**new_env;
	int		i;
	int		count;

	count = 0;
	while (env[count])
		count++;
	
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	
	i = 0;
	while (i < count)
	{
		new_env[i] = strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	
	return (new_env);
}