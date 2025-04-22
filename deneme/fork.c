#include "cell.h"
#include <sys/types.h>

int main()
{
    pid_t parent_pid = getpid(); // Ebeveynin PID'si (getpid pid_t döndürür)
    pid_t id; // fork'un dönüş değeri için

    printf("Fork öncesi - Ebeveyn PID: %d\n", parent_pid);

    id = fork();

    if (id < 0)
    {
        // Hata durumu: fork başarısız oldu
        perror("fork başarısız");
        exit(EXIT_FAILURE);
    }
    else if (id == 0)
    {
        // Bu blok SADECE ÇOCUK İŞLEM tarafından çalıştırılır
        printf("--> Burası CHILD process. Benim PID'im: %d, Ebeveynimin PID'i: %d\n",
               getpid(), getppid()); // getppid() ebeveynin PID'sini verir
    }
    else // id > 0
    {
        // Bu blok SADECE EBEVEYN İŞLEM tarafından çalıştırılır
        // 'id' değişkeni burada çocuğun PID'sini içerir
        printf("<-- Burası MAIN (PARENT) process. Benim PID'im: %d, Oluşturduğum çocuğun PID'i: %d\n",
               getpid(), id);
    }

    // Bu satır hem ebeveyn hem de çocuk tarafından çalıştırılır (eğer çocuk exit() ile çıkmazsa)
    // printf("Fork sonrası - PID: %d, fork() dönüş değeri (id): %d\n", getpid(), id);

    return 0;
}
/*int main()
{
	int pid = getpid();
	int id = fork();

//	printf("forksonrası, %d\n", id);
	if (id == pid)
	{
		printf("burası main process");
	}
	else if (id == 0)
	{
		printf("burası child process");
	}
}*/
