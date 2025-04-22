#include "cell.h"

int main(int argc, char **argv)
{
	int status;

	if (fork() == 0)
		execvp(argv[1], argv + 1);

	wait(&status);

	return (0);
}
