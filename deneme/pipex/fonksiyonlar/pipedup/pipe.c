#include "cell.h"
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
 * bu filedescriptor entrylere point edecek şekilde ayarlar. 
 */


int main(int argc, char **argv, char **env)
{
	int pipefd[2];
	char buffer[100];
	pid_t pid;

	printf("%d", pipefd[0]);
	pipe(pipefd);

	printf("%d", pipefd[0]);
}
