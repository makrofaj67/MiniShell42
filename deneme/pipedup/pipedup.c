#include "cell.h"

void	first_child_process(int pipefd[2], char **envp)
{
	char	*cmd1_args[] = {"/bin/ls", "-la", NULL};

	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	execve(cmd1_args[0], cmd1_args, envp);
}

void	second_child_process(int pipefd[2], char **envp)
{
	char	*cmd2_args[] = {"/usr/bin/grep", ".txt", NULL};

	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	execve(cmd2_args[0], cmd2_args, envp);
}

int main(int argc, char **argv, char **env)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	pipe(pipefd);
	pid1 = fork();
	if (pid1 == 0)
		first_child_process(pipefd, env);

	pid2 = fork();
	if (pid2 == 0)
		second_child_process(pipefd, env);

	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0); 	
	waitpid(pid2, &status, 0);	
	return (EXIT_SUCCESS);
}
