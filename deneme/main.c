#include "cell.h"

int main(int argc, char **argv)
{

	if (fork() == 0)
		execvp(argv[1], argv + 1);

	wait(NULL);

	return (0);
}
