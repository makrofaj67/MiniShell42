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

/*
 *
 *Evet, Pipe'ın varsayılan olarak "blocking" (engelleyici) modda çalıştığını ve bu durumun "non-blocking" (engellemeyen) moddan farklı olduğunu söylemiştim. Şimdi bu iki mod arasındaki farkı ve non-blocking modun nasıl çalıştığını açıklayalım.
Blocking (Engelleyici) Mod (Pipe'ların Varsayılanı)
Pipe'lar (ve genellikle diğer I/O işlemleri de) varsayılan olarak blocking moddadır. Bu şu anlama gelir:
    read() (Okuma): Bir process Pipe'ın okuma ucundan read() çağırdığında ve Pipe'ta okunacak veri yoksa (tampon boşsa), read() çağrısı ve çağıran process bloklanır (durdurulur). İşletim sistemi, Pipe'a veri yazılana veya Pipe'ın yazma ucu tamamen kapatılana kadar process'i bekletir. Veri geldiğinde veya yazma ucu kapanınca read bloğu kalkar, veri okunur (veya 0 döndürülür) ve process devam eder.
    write() (Yazma): Bir process Pipe'ın yazma ucuna write() çağırdığında ve Pipe'ın tamponu doluysa (yani yazılacak veriyi alacak yer yoksa ve okuyucu veriyi yeterince hızlı tüketmiyorsa), write() çağrısı ve çağıran process bloklanır. Pipe tamponunda yer açıldığında (başka bir process Pipe'tan okuduğunda), write bloğu kalkar, veri yazılır ve process devam eder. Ayrıca, eğer Pipe'ın okuma ucu tamamen kapatılmışsa (yani yazılan veriyi okuyacak kimse kalmadıysa), yazma işlemi başarısız olur, genellikle SIGPIPE sinyali gönderilir ve write çağrısı EPIPE hatasıyla döner.
Blocking mod, processler arası senkronizasyonun doğal bir yoludur. Okuyucu, yazıcı hazır olana kadar bekler; yazıcı, okuyucu hazır olana kadar bekler.
Non-blocking (Engellemeyen) Mod
Pipe'ları veya diğer I/O kaynaklarını non-blocking modda açtığınızda (bu genellikle fcntl() fonksiyonu ile O_NONBLOCK bayrağı set edilerek yapılır), read ve write çağrıları varsayılan blocking davranışını değiştirir:
    read() (Non-blocking): Bir process Pipe'ın okuma ucundan read() çağırdığında ve Pipe'ta okunacak veri yoksa, read() çağrısı hemen geri döner. Bloklanmaz. Döndürdüğü değer -1 olur ve errno global değişkeni EAGAIN veya EWOULDBLOCK olarak ayarlanır. Bu, "şu anda veri yok, daha sonra tekrar dene" anlamına gelir. Eğer veri varsa, normal bir şekilde okuma yapar ve okuduğu byte sayısını döndürür.
    write() (Non-blocking): Bir process Pipe'ın yazma ucuna write() çağırdığında ve Pipe'ın tamponu doluysa, write() çağrısı hemen geri döner. Bloklanmaz. Döndürdüğü değer -1 olur ve errno EAGAIN veya EWOULDBLOCK olarak ayarlanır. Bu, "şu anda tampon dolu, daha sonra tekrar dene" anlamına gelir. Eğer tamponda yer varsa, normal bir şekilde yazma yapar ve yazdığı byte sayısını döndürür (Pipe tamponuna tam sığmayan bir write'ın bir kısmını yazıp dönme ihtimali de vardır). Okuma ucu kapalıysa yine SIGPIPE/EPIPE davranışı geçerlidir.
Nasıl Çalışıyor (Teknik Biraz):
Non-blocking mod, işletim sistemine bir ipucudur. Sen non-blocking bayrağını set ettiğinde, çekirdek o File Table Entry ile ilgili I/O çağrılarında process'i uykuya yatırmak yerine hemen hata kodu ile dönmesi gerektiğini bilir.
Non-blocking Mode Ne Zaman Kullanılır?
Non-blocking I/O genellikle şu senaryolarda kullanılır:
    Tek Bir Process'in Birden Çok Kaynağı Yönetmesi: Bir process'in aynı anda hem ağ soketlerinden, hem Pipe'lardan, hem de diğer dosyalardan veri okuması veya yazması gerektiğinde. Blocking modda olsaydı, herhangi bir kaynaktan okuma/yazma denemesi sırasında veri gelmeyince tüm process bloklanırdı. Non-blocking modda ise read veya write hemen döner, process başka bir kaynağı kontrol etmeye devam edebilir.
    Polling (Yoklama): Bir process'in sürekli olarak bir kaynaktan veri gelip gelmediğini kontrol etmesi gerektiğinde (select, poll, epoll gibi sistem çağrılarıyla birlikte).
    Zaman Aşımlı (Timeout) I/O: Belirli bir süre içinde I/O işlemi gerçekleşmezse vazgeçilmesi gereken durumlarda.
Pipex Projesi ve Non-blocking:
Pipex projesinin zorunlu (mandatory) kısmında Pipe'ları non-blocking modda kullanman gerekmez. Varsayılan blocking davranış, cmd1 | cmd2 gibi basit bir pipeline senaryosu için yeterlidir ve hatta işleri basitleştirir: cmd2 veri gelene kadar bekler, cmd1 tampon dolana kadar bekler. Bu doğal bir senkronizasyon sağlar.
Non-blocking I/O, genellikle daha karmaşık I/O yönetimi gerektiren (örneğin bir server veya GUI uygulaması gibi) projelerde kullanılır.
Umarım bu açıklama, blocking ve non-blocking I/O arasındaki farkı ve non-blocking modun temel çalışma prensibini netleştirmiştir. Pipex için blocking I/O ile devam edebilirsin, bu yeterli olacaktır.
 * */


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
