#include "cell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv, char **env)
{	
	int fdtoin;
	
	fdtoin = open("output.txt", O_RDONLY);

	char *args[] = {
		"/usr/bin/grep",
		"saas",
		NULL
	};

	dup2(fdtoin, STDIN_FILENO);
	//burada artık progra stdinden değil, fdtoinden okuma yapacak.
	//
	execve(args[0], args, env);
}

