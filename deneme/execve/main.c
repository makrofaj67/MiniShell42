#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv, char **env)
{
	char *args[] = {
		"/usr/bin/ls",
		"-la",
		NULL
	};

	perror("hii");
	execve(args[0], args, env);
}
