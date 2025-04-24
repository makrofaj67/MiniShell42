/*
"Bir process'in kendine özel File Descriptor Table'ı, File Table Entry'lere işaret
eden pointer'lardan oluşan bir dizidir. File Descriptor integer'ı, bu dizideki bir
index (dizin) numarasıdır. Programcı (veya sistem çağrısı) bu index numarasını
kullanarak o dizideki ilgili girişe erişir ve bu girişin içerdiği File Table Entry
pointer'ına ulaşır."

"Processlerin fdlerine /proc/[pid]/fd yolundan erişebilirsin.

"file table entry:

1) current file offset = read and write için ortak olan, bir sonraki işlemin nereden
başlayacağının yerini tutan işaretçi. fork() yapıldığı zaman file descriptor table
child processe aynen kopyalanır. iki ayrı file descriptor table oluşur. ancak içerideki 
file descriptor entry pointerları hala aynı fdentrye işarete ettiği için, read write gibi
şeyler ortak yerden okunur.""
2) open file status flag = dosyanın hangi modda açıldığını gösterir.
.   O_RDONLY | O_WRONLY | O_RDWR | O_CREAT | O_TRUNC | O_APPEND
	O_RDONLY: Sadece okuma için açıldı.
	O_WRONLY: Sadece yazma için açıldı.
	O_RDWR: Hem okuma hem yazma için açıldı
	O_APPEND: Yazma işlemleri her zaman dosyanın sonuna yapılır
(offset'in otomatik olarak dosya sonuna taşınmasını sağlar).
	O_NONBLOCK: Okuma/yazma işlemleri engellemeden (non-blocking) yapılır.
3) Access Mode = Yukarıdaki bayrakların bir özetidir; dosyanın okuma,
yazma veya her ikisi için mi açık olduğunu belirtir.
4) Reference Count = Bu File Table Entry'ye kaç tane File Descriptor'ın (farklı process'lerde veya aynı process'te) işaret ettiğini tutan bir sayaçtır.
    Bir open() çağrıldığında (ve yeni bir Fd ve FTe oluşturulursa), bu sayı 1 olur.
    Bir fork() çağrıldığında, ebeveynin tüm açık dosyaları çocukta kopyalanır, bu da işaret edilen File Table Entry'lerin referans sayısını her bir FD için artırır.
    Bir dup() veya dup2() çağrıldığında, yeni oluşturulan FD'nin işaret ettiği File Table Entry'nin referans sayısı artırılır.
    Bir close() çağrıldığında, ilgili FD'nin işaret ettiği File Table Entry'nin referans sayısı azaltılır.
    Referans sayısı 0'a düştüğünde (yani hiçbir File Descriptor artık bu File Table Entry'ye işaret etmediğinde), çekirdek bu File Table Entry'yi bellekten kaldırır.
5) Pointer to Inode Structure = Bu File Table Entry'nin temsil ettiği dosyanın gerçek meta verilerini (izinler, sahip, boyut, diske üzerindeki veri bloklarının adresleri vb.) tutan Inode yapısına bir işaretçidir. Inode, dosyanın kendisini tanımlayan yapıdır. Aynı dosya birden fazla farklı modda açılsa veya birden fazla File Table Entry tarafından temsil edilse bile, hepsi aynı Inode yapısına işaret eder.

bitwise opening operations
| işareti burada bitwise OR operatörüdür ve tam olarak senin dediğin gibi, O_WRONLY ve O_TRUNC bayraklarının birleştirilerek open() fonksiyonuna tek bir tamsayı değeri olarak geçirilmesini sağlıyor.
Bitwise OR (|) Ne Yapar?
İşletim sistemleri ve sistem programlamada bayraklar (flags) genellikle tamsayılar olarak temsil edilir ve her bayrak, o tamsayının bir belirli bitini set eder. Örneğin:
    O_RDONLY değeri, diyelim ki 00000001 (ikilik tabanda) olsun.
    O_WRONLY değeri, diyelim ki 00000010 olsun.
    O_CREAT değeri, diyelim ki 00000100 olsun.
    O_TRUNC değeri, diyelim ki 00001000 olsun.
    Ve diğer bayraklar da farklı bitleri set eder.
Bu bayrakların değerleri, birbirleriyle çakışmayacak şekilde (her biri farklı bir bit konumunu temsil edecek şekilde) dikkatlice seçilmiştir.
Sen O_WRONLY | O_TRUNC yazdığında, C derleyicisi bu iki sayının bitwise OR işlemini yapar.
Örnek (sayılar temsilidir):
O_WRONLY  :  00000010
| O_TRUNC   :  00001000
----------------------
Sonuç       :  00001010


trunc işlemiyle dosya açıldığında, eğer içerisinde veri varsa tamamen silinir. > redirectionlarında işimize yarayacak. sadece wrongly ve rdwr ile tetiklenir.

*/

#include "cell.h"
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv, char **env)
{
	int fd;

	fd = open("deneme.txt", O_RDWR);
	printf("fd = %d", fd);

	char *towrite;

	towrite = "Merhaba Dünya!!";
	write(fd, towrite, strlen(towrite));

	
	char buf[22];
	int readed_bytes = 1;

	while(readed_bytes > 0)
	{
		readed_bytes = read(fd, buf, 3);
		buf[readed_bytes] = '\0';
		printf("%s", buf);
		fflush(0);
	}

	close(fd);

	fd = open("deneme.txt", O_WRONLY | O_TRUNC);

}

