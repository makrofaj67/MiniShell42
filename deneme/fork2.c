#include "cell.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("forköncesi\n");

	int id = fork();

	if (id == 0)
	{
		printf("++++++++++++++++++++++\n");
		printf("ben child processim\n");
		printf("child process pid is=%d\n", getpid());
		printf("parent process pid is=%d\n", getppid());
		printf("++++++++++++++++++++++\n");
	}
	if (id > 0)
	{
		wait(NULL); //stop the execution until child process executed memorysi tamamen deallocate olana kadar da denebilir.
		printf("-----------\n");
		printf("ben parent processim\n");
		printf("child process pid is=%d\n", id);
		printf("parent process pid is=%d\n", getpid());
		printf("-----------\n");
	}
}
