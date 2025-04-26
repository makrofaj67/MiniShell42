#include "cell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv, char **env)
{
	int fdtout;

	fdtout = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	//creat kullanıldığında izin flaglarının yazılması gereklidir.
	
	char *args[] = {
		"/usr/bin/ls",
		"-la",
		NULL
	};

	close(STDOUT_FILENO);
	//stdout_fileno indexindeki pointer artık herhangi bir fteyi işaret etmez
	dup2(fdtout, STDOUT_FILENO);
	/*
	 * 
	 * burada stdout_fileno bir file descriptordur, integerdir. file descriptor
	 * tableda bir file descriptor table entry pointerı indexidir. biz bu index
	 * ile bir ftepointerına erişiriz.
	 *
	 * dup2 ile stdout_fileno indexindeki pointerı güncelleriz. stdout indexindeki
	 * pointerın işaret ettiği fte'yi, fdtodup indexindeki pointerın işaret ettiği
	 * fte ile değiştiriz. böylece stdout_fileno indexiyle point edilen fte de
	 * referans sayısı bir azalır.
	 *
	 * ftodupun işaret ettiği ftede ise referans bir artar. program
	 * hala stdout_fileno indexindeki pointerın işaret ettiği fte ye output vermektedir.
	 * ancak bu şu an stdout değil, fdtodup olduğu için veriler fdtodupa yazılır.
	 *
	 *
	*/
	execve(args[0], args, env);
}
