#include "cell.h"

void	first_child_process(int pipefd[2], char **envp)
{
	char	*cmd1_args[] = {"/bin/ls", "-la", NULL};

	close(pipefd[0]); 
	dup2(pipefd[1], STDOUT_FILENO);
	//burada dup2 ile pipefd[1] e şu an içinde bulunduğumuz processin
	//stdoutu yazdırılır gibi bir şey. ancak execve gerçekleşmeden 
	//pipefd[1] kapatılıyor burayı tam çözemedim.
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
	/*
	 * Şu an kernel tarafından, ana processın file descriptor tableında iki adet
	 * file descriptor table entry pointer oluşturuldu. bu pointerlara pipefd üzerindeki
	 * file descriptor integerları -indexleri- sayesinde erişebiliyoruz. bu pointerlarla
	 * eriştiğimiz fileların biri yazma diğeri okuma için kullanılacak. yazma tarafından
	 * yazılan her şey otomatik olarak okuma tarafındaki file'a yazılacak. yine bu otomatk
	 * olarak kernel tarafından gerçekleşen bir şey. Burada kernel tarafından öncelikle
	 * file descriptor table entryler oluşturulur, ardından processin file descriptor tableına
	 * kernel tarafından bu table entrye işaret eden pointerlar konur. biz de int = fd
	 * den gelen indexle bu pointerlara erişiriz.
	*/
	//elimizde sadece 1 process var.
	pid1 = fork();	
	//şu an elimizde 1 ana process ve 1 child process var
	/*
	 * Bu iki processin tek farkı pid1 numaraları, birisi 0 diğer ise 0 dan başka bir şeydir.
	 * yine bu ayrımı kernel yapar, aynı integerı farklı processlerde farklı şekilde görebilmemizin
	 * sebebi bu. bu iki işlemin bağımsız file tableları da oluşur. bağımsız olmalarına rağmen file
	 * descriptor table entry pointerları yine aynı filelara point eder, ki bunların arasında bizim
	 * oluşturduğumuz pipfd dizisinde pipe için oluşturduğumuz iç kaynaklar da bulunur. yani birden
	 * fazla file descriptor table ve birden fazla file descriptor table entry pointerı olmasına rağmen
	 * bu pointerlarla erişilen filelar (burada pipefd dizisindeki dosyalar oluyor), birer tanedir onlar 
	 * duplicate olmaz. Bu file descriptor table pointer dizisi her processe özeldir, kendi memorylerinde bulunur.
	 * Ancak file descriptor table entry pointerlarının işaret ettiği table entryler processin içerisinde bulunmaz.
	 * Processin bellek alanı dışında, çekirdek belleğinde (kernel space) içerisinde yer alır ve çekirdek tarafından
	 * yönetilir. Bu yüzden fork ile yeniden oluşturulmaz.
	 */
	if (pid1 == 0)
		//child processi execve ile ls'e dönüştürdük.
		//forkladığımız yeni child processte de şu an pipfed[2] pointerları aynı şekilde mevcut.
		//ana processte table entry pointerlarla eriştiğimiz dosyalar tamemen aynı ve process memorylerinin içinde
		//değil kernel space içerisinde bulunur. pipe edilmiş iki dosyal ile iletişimi sağlayacağız.
		first_child_process(pipefd, env);
		//execve ile dönüştürüldükten sonra processin yaşam döngüsü bitti
	//ve öldü, elimizde yine sadece 1 adet parent kaldı.

	//elimizde 1 adet parent process var.
	pid2 = fork();
	if (pid2 == 0)
		//child processi execve ile grepe'e dönüştürdük.
		second_child_process(pipefd, env);
		//execve ile dönüştürüldükten sonra processin yaşam döngüsü bitti
	//ve öldü, elimizde yine sadece 1 adet parent kaldı.

	//elimizde 1 adet parent kaldı. //toplamda 3 adet process oluştu.
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0); 	
	waitpid(pid2, &status, 0);	
	return (EXIT_SUCCESS);
}
