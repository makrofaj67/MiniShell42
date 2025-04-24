#include "cell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*
 * pipe'ın amacı iki process arasında veri alışverişi yapmaktır. pipe systemcall'u
 * parametre olarak iki tane file descriptor alır, ancak bu kafa karışıklığı oluşturabilir.
 * amaç işlemlerin ulaşabileceği herhangi bir fiziksel dosyaya bir işlemden veri yazıp,
 * diğer işlemden bunu okutmak değildir. amaç sadece işlemler arası veri aktarmaktır.
 * bu yüzden kernel işlemler sürecinde var olan -on the fly mevcudiyet- dosyalar oluşturup,
 * bunlar ile iletişim kurar. biz bu dosyaların nerede olduğunu önemsemeyiz, aslında
 * dosya oluşturulmasını da önemlemeyiz, bu dosyaların sadece veri akışını kolaylaştırması
 * için kernel tarafından on the fly oluşturulduğunu biliriz. bizim için dosyalar önemli
 * olmadığı için, kernel pipe syscallu sırasında dosyaları kendisi oluşturur. pipe syscall
 * ının istediği tek şey en az iki integer saklayabileceği bir integer dizisidir. bu dizide
 * önceden ne yazıldığı önemli değildir. pipe() syscallı aldığı pipefd dizisinin bellekteki
 * adresini alır.
 *
 * ardından iki adet file descriptor table entry oluşturur. bunları processteki en düşük
 * file descriptordeki uygun yere koyar. ardından ilkin aldığı pipefd dizisindeki fdleri
 * bu filedescriptor entrylere point edecek şekilde ayarlar. yani pipefd[2] içerisindeki
 * başlangıç değerlerinin önemi kalmaz.
 *
 * fd tablosu pointer dizisidir, fd integerı dizinin indexidir.
 */


int main(int argc, char **argv, char **env)
{
	int pipefd[2];
	char buffer[100];
	pid_t pid;

	pipe(pipefd);

	pid = fork();
	if (pid == 0)
	{
		close(pipefd[1]); // child process sadece okuyacak, yazma ucu kapatılır.

		int readed_bytes = 1;

		while(readed_bytes > 0)
		{
			readed_bytes = read(pipefd[0], buffer, 3);
			buffer[readed_bytes] = '\0';
			printf("%s", buffer);
			fflush(0);
		}
		close(pipefd[0]);
	}
	else
	{	
		close(pipefd[0]); // parent process sadece yazacak, okuma ucu kapatılır.

		char *message_to_send = "Merhaba bu mesajı gönderen parent process. Ben ise pipe ile bu mesajı alan child processim. Pipeın ucundan okuyorum, stdouta yazdırıyorum.";
		int len = strlen(message_to_send);
		
		write(pipefd[1], message_to_send, len);
		
		close(pipefd[1]);
		wait(NULL);
	}
}
