#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit, EXIT_FAILURE, EXIT_SUCCESS, free, malloc
#include <string.h>     // strcmp, strtok
#include <time.h>
#include <unistd.h>     // fork, execve, getenv
#include <sys/wait.h>   // wait, waitpid
#include <sys/types.h>  // pid_t
#include <readline/readline.h>
#include <readline/history.h> // add_history için


/*

execve(programıntamyolu, komutsatırıargümanları, enviromentvariables);
	
*/

int main(int argc, char **argv, char **env)
{

	char *args[] = {
		"/usr/bin/ls",
		NULL,
		NULL
	};
	execve("/usr/bin/cat", args, NULL);

}
