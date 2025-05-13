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

#include "../../inc/c_parser.h"
#include "../../inc/d_executor.h"
#include "../../inc/f_builtins.h"

void	execute_path(char *path, char **args)
{
	int		pid;

	pid = fork();
	if (pid == -1)
	{
        perror("minishell: fork");
        return;
    }
	else if (pid == 0) 
	{
        if (execve(path, args, NULL) == -1)
		{
            write(2, "execve error\n", 13);
            exit(1);
        }
    }
	else
        wait(NULL);
}

char *find_pathx(char *cmd)
{
    if (!cmd || cmd[0] == '\0') {
        write(2, "Error: Command is NULL or empty.\n", 33);
        return NULL;
    }

    char *path = getenv("PATH");
    if (!path) {
        write(2, "Error: PATH environment variable not set.\n", 42);
        return NULL;
    }

    char dir_path[1024];
    int i = 0, j = 0;

    while (path[i] != '\0') {
        j = 0;
        while (path[i] != ':' && path[i] != '\0') {
            dir_path[j++] = path[i++];
        }
        dir_path[j] = '\0'; // Null-terminate the directory path

        // Construct the full command path
        char *cmd_path = malloc(mini_strlen(dir_path) + mini_strlen(cmd) + 2);
        if (!cmd_path) {
            write(2, "Error: Memory allocation failed.\n", 34);
            return NULL;
        }
        sprintf(cmd_path, "%s/%s", dir_path, cmd);

        // Check if the command is executable
        if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) == 0) {
            return cmd_path; // Return the valid command path
        }

        free(cmd_path); // Free the memory if the command is not valid

        if (path[i] == ':') i++; // Skip the ':' separator
    }

    // Command not found in any directory
    write(2, "Error: Command not found in PATH.\n", 34);
    return NULL;
}



int execute_command_node(ast_node *node, int *exit_status, t_env **env, t_env **env_var)
{
    char **arg_array = node->value->arg_array;
    t_redirection **redirections = node->value->redirections;
    int i;
    char *path;

    i = 0;
	if (!node || !node->value)
    {
        printf("Error: Command node or its value is NULL.\n");
        return -1;
    }
    if (!arg_array || !arg_array[0])
    {
        printf("Error: Command is NULL.\n");
        return -1;
    }
    i = builtin_selector(arg_array, env, env_var);
    // Find the path for the command
    if (i == 1)
    {
        path = find_pathx(arg_array[0]);
        if (!path)
        {
            printf("Error: Command not found: %s\n", arg_array[0]);
            return -1;
        }
        execute_path(path, arg_array);        
        free(path);
    }

/*
    // Arg array yazdırma
    printf("Arguments array:\n");
    while (arg_array && arg_array[i])
    {
        printf("  [%d]: %s\n", i, arg_array[i]);
        i++;
    }

    // Redirections yazdırma
    printf("Redirections:\n");
    i = 0;
    while (redirections && redirections[i])
    {
        const char *redir_type_name;
        switch (redirections[i]->type)
        {
            case REDIR_IN:
                redir_type_name = "<"; // Input redirection
                break;
            case REDIR_OUT:
                redir_type_name = ">"; // Output redirection
                break;
            case REDIR_APPEND:
                redir_type_name = ">>"; // Append redirection
                break;
            case REDIR_HEREDOC:
                redir_type_name = "<<"; // Heredoc redirection
                break;
            default:
                redir_type_name = "Unknown"; // Unknown redirection type
                break;
        }
        printf("  [%d]: Type: %s, Filename: %s\n", i, redir_type_name, redirections[i]->filename);
        i++;
    }
		*/
}
int execute_pipe_node(ast_node *node, int *exit_status, t_env **env, t_env **env_var)
{

	//ast_node rightta pipeın right tarafını execute_command_ile execleyebilirsin
    {
		if (!node)
		{
			printf("Error: Pipe node is NULL.\n");
			return -1;
		}
	
		// Sol taraftaki komut veya pipe
		if (node->left)
		{
			if (node->left->type == PIPE_NODE)
			{
				printf("Left Pipe:\n");
				execute_pipe_node(node->left, exit_status, env, env_var); // Rekürsif çağrı
			}
			else
			{
				printf("Left Command:\n");
				execute_command_node(node->left, exit_status, env, env_var);
			}
		}
	
		// Sağ taraftaki komut
		if (node->right)
		{
			printf("Right Command:\n");
			execute_command_node(node->right, exit_status, env, env_var);
		}
	
		return 0; // Başarılı çalışmayı belirtmek için 0 döndür

	}
}
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
	execute_ast(root_node, exit_status, env, env_var);
}