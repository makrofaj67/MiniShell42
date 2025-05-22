/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 01:52:25 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 01:52:25 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char *find_pathx(char *cmd)
{
	if (!cmd || cmd[0] == '\0')
		return NULL;
	char *path = getenv("PATH");
	if (!path)
		return NULL;
	char dir_path[1024];
	int i = 0, j = 0;

	while (path[i] != '\0') {
		j = 0;
		while (path[i] != ':' && path[i] != '\0') {
			dir_path[j++] = path[i++];
		}
		dir_path[j] = '\0';
		char *cmd_path = malloc(mini_strlen(dir_path) + mini_strlen(cmd) + 2);
		if (!cmd_path)
			return NULL;
		sprintf(cmd_path, "%s/%s", dir_path, cmd);
		if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) == 0)
			return cmd_path;
		free(cmd_path);
		if (path[i] == ':') i++;
	}
	return NULL;
}

void	util_helper(t_redirection **redirections, char **arg_array, t_env **env, t_env **env_var, int *i)
{
	*i = 0;
    while (redirections && redirections[*i])
	{
		if (redirections[*i]->type == REDIR_HEREDOC)
			heredoc_helper(redirections[*i]);
		(*i)++;
	}
	*i = builtin_selector(arg_array, env, env_var);
	if (arg_array[0] == NULL || arg_array == NULL)
		exit(0);
}

int		execute_command_node(ast_node *node, int *exit_status, t_env **env, t_env **env_var)
{
	char **arg_array = node->value->arg_array;
	t_redirection **redirections = node->value->redirections;
	int i;
	char *path;
	int pid;

	util_helper(redirections, arg_array, env, env_var, &i);
	pid = fork();
	if (pid == 0)
	{
		if (redirections && redirections[0])
			handle_redirections(redirections);
		if (i == 1)
		{
			path = find_pathx(arg_array[0]);
			if (!path)
				return -1;
			execve(path, arg_array, NULL);
            free(path);
		}
		exit(*exit_status);
	}
	waitpid(pid, NULL, 0);
	return (0);
}

/*********************/
int execute_pipe_node(ast_node *node, int *exit_status, t_env **env, t_env **env_var)
{
	int fd[2];
	int pid1;
	int pid2;

	if (pipe(fd) == -1)
		return (-1);
	if (!node)
		return -1;
	pid1 = fork();
	if (pid1 == -1)
		return (-1);
	if (pid1 == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		if (node->left->type == PIPE_NODE)
			execute_pipe_node(node->left, exit_status, env, env_var);
		else
			execute_command_node(node->left, exit_status, env, env_var);
		exit(0);
	}
	pid2 = fork();
	if (pid2 == -1)
		return (-1);
	if (pid2 == 0)
	{
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		execute_command_node(node->right, exit_status, env, env_var);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return 0;
}

/********************/
int execute_ast(ast_node *node, int *exit_status, t_env **env, t_env **env_var)
{
	if (node == NULL)
		return 0;
	if (node->type == COMMAND_NODE)
		return (execute_command_node(node, exit_status, env, env_var));
	if (node->type == PIPE_NODE)
		return (execute_pipe_node(node, exit_status, env, env_var));
	return (0);
}
void execute_command_root(ast_node *root_node, int *exit_status, t_env **env, t_env **env_var)
{
	if (root_node == NULL)
		return ;
	if (execute_ast(root_node, exit_status, env, env_var) == -1)
		return ;
}