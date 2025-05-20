
#include "../../inc/__minishell.h"

static int setup_redirections(command_value *cmd_details)
{
	int	i;
	int	fd;

	i = 0;
	while (cmd_details->redirections && cmd_details->redirections[i])
	{
		if (cmd_details->redirections[i]->type == REDIR_IN)
		{
			fd = open(cmd_details->redirections[i]->filename, O_RDONLY);
			if (fd < 0)
			{
				perror(cmd_details->redirections[i]->filename);
				return (-1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (cmd_details->redirections[i]->type == REDIR_OUT)
		{
			fd = open(cmd_details->redirections[i]->filename,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror(cmd_details->redirections[i]->filename);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (cmd_details->redirections[i]->type == REDIR_APPEND)
		{
			fd = open(cmd_details->redirections[i]->filename,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror(cmd_details->redirections[i]->filename);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		i++;
	}
	return (0);
}

static int execute_simple_command(ast_node *node, int *exit_status,
	t_env **env_list, t_env **export_list)
{
	pid_t	pid;
	int		status;
	char	**env_array;
	char	*full_path;

	if (!node->value->arg_array || !node->value->arg_array[0])
		return (0);

	if (is_builtin(node->value->arg_array[0]))
	{
		if (setup_redirections(node->value) < 0)
			return (1);
		int stdin_copy = dup(STDIN_FILENO);
		int stdout_copy = dup(STDOUT_FILENO);
		
		*exit_status = execute_builtin(node->value->arg_array, env_list, export_list);
		
		dup2(stdin_copy, STDIN_FILENO);
		dup2(stdout_copy, STDOUT_FILENO);
		close(stdin_copy);
		close(stdout_copy);
		
		return (*exit_status);
	}

	pid = fork();
	if (pid == 0)
	{
		if (setup_redirections(node->value) < 0)
			exit(1);
		
		env_array = env_to_array(*env_list);
		
		if (node->value->arg_array[0][0] == '/' ||
			node->value->arg_array[0][0] == '.')
		{
			if (execve(node->value->arg_array[0], node->value->arg_array, env_array) < 0)
			{
				perror(node->value->arg_array[0]);
				exit(127);
			}
		}
		else
		{
			full_path = find_command_in_path(node->value->arg_array[0], *env_list);
			if (!full_path)
			{
				fprintf(stderr, "minishell: %s: command not found\n",
					node->value->arg_array[0]);
				exit(127);
			}
			if (execve(full_path, node->value->arg_array, env_array) < 0)
			{
				perror(node->value->arg_array[0]);
				free(full_path);
				free_array(env_array);
				exit(127);
			}
		}
	}
	else if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*exit_status = 128 + WTERMSIG(status);
	
	return (*exit_status);
}

static int execute_heredoc(t_redirection *redir)
{
	char	*line;
	int		fd;
	char	tmp_file[32];
	
	strcpy(tmp_file, "/tmp/minishell_heredoc_XXXXXX");
	fd = mkstemp(tmp_file);
	if (fd < 0)
	{
		perror("heredoc");
		return (-1);
	}
	
	unlink(tmp_file);
	
	setup_heredoc_signals();
	
	reset_signal_flag();
	
	while (1)
	{
		line = readline("> ");
		
		if (g_signal_received == SIGINT)
		{
			free(line);
			close(fd);
			reset_signal_flag();
			setup_interactive_signals();
			return (-1);
		}
		
		if (!line || (redir->filename && !strcmp(line, redir->filename)))
		{
			free(line);
			break;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	
	setup_interactive_signals();
	
	lseek(fd, 0, SEEK_SET);
	return (fd);
}

static int process_heredocs(command_value *cmd_details)
{
	int	i;
	int	fd;
	int	stdin_copy;

	stdin_copy = dup(STDIN_FILENO);
	if (stdin_copy < 0)
	{
		perror("dup");
		return (-1);
	}
	
	i = 0;
	while (cmd_details->redirections && cmd_details->redirections[i])
	{
		if (cmd_details->redirections[i]->type == REDIR_HEREDOC)
		{
			fd = execute_heredoc(cmd_details->redirections[i]);
			if (fd < 0)
			{
				dup2(stdin_copy, STDIN_FILENO);
				close(stdin_copy);
				return (-1);
			}
			
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		i++;
	}
	
	cmd_details->stdin_backup = stdin_copy;
	return (0);
}

static int execute_pipeline(ast_node *node, int *exit_status,
	t_env **env_list, t_env **export_list)
{
	int		pipe_fds[2];
	pid_t	left_pid, right_pid;
	int		status;

	if (pipe(pipe_fds) < 0)
	{
		perror("pipe");
		return (1);
	}

	left_pid = fork();
	if (left_pid == 0)
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
		
		execute_ast_node(node->left, exit_status, env_list, export_list);
		exit(*exit_status);
	}
	else if (left_pid < 0)
	{
		perror("fork");
		return (1);
	}

	right_pid = fork();
	if (right_pid == 0)
	{
		close(pipe_fds[1]);
		dup2(pipe_fds[0], STDIN_FILENO);
		close(pipe_fds[0]);
		
		execute_ast_node(node->right, exit_status, env_list, export_list);
		exit(*exit_status);
	}
	else if (right_pid < 0)
	{
		perror("fork");
		return (1);
	}

	close(pipe_fds[0]);
	close(pipe_fds[1]);
	
	int left_status;
	int right_status;
	
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	
	if (WIFEXITED(right_status))
		*exit_status = WEXITSTATUS(right_status);
	else if (WIFSIGNALED(right_status))
		*exit_status = 128 + WTERMSIG(right_status);
	
	return (*exit_status);
}

int execute_ast_node(ast_node *node, int *exit_status,
	t_env **env_list, t_env **export_list)
{
	int result;
	
	if (!node)
		return (0);

	if (node->type == COMMAND_NODE)
		result = execute_simple_command(node, exit_status, env_list, export_list);
	else if (node->type == PIPE_NODE)
		result = execute_pipeline(node, exit_status, env_list, export_list);
	else
		result = 0;
	
	if (node->type == COMMAND_NODE && node->value && node->value->stdin_backup > 0)
	{
		dup2(node->value->stdin_backup, STDIN_FILENO);
		close(node->value->stdin_backup);
		node->value->stdin_backup = -1;  
	}
	
	return (result);
}

int execute_ast(ast_node *root_node, int *exit_status,
	t_env **env_list, t_env **export_list)
{
	struct sigaction	sa_int, sa_quit;
	struct sigaction	old_sa_int, old_sa_quit;
	int					result;

	sigaction(SIGINT, NULL, &old_sa_int);
	sigaction(SIGQUIT, NULL, &old_sa_quit);
	
	sa_int.sa_handler = SIG_DFL;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	
	sa_quit.sa_handler = SIG_DFL;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
	
	if (root_node->type == COMMAND_NODE)
		process_heredocs(root_node->value);
	
	result = execute_ast_node(root_node, exit_status, env_list, export_list);
	
	sigaction(SIGINT, &old_sa_int, NULL);
	sigaction(SIGQUIT, &old_sa_quit, NULL);
	
	return (result);
}

