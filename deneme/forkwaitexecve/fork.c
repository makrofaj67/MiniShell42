#include "cell.h"
#include <sys/types.h>


/*


----------------------

Harika bir soru! Cevap kesinlikle evet, fork() sistem çağrısı çağrılmadan önceki program durumu da child (çocuk) programa dahil edilir (kopyalanır).

Düşünmeniz gereken şey şu: fork() anlık bir "fotokopi çekme" işlemi gibidir.

Fotokopi Anı: fork() sistem çağrısı yapıldığı anda, işletim sistemi çekirdeği, çağrıyı yapan ebeveyn (parent) sürecin o anki tam durumunu (process image) alır.

Kopyalananlar: Bu "tam durum" şunları içerir:

Bellek Alanı:

Kod Segmenti: Programın makine kodları. (Bu genellikle paylaşılır, kopyalanmaz çünkü değişmez).

Veri Segmenti: Global ve statik değişkenlerin o anki değerleri. fork()'tan önce bu değişkenlere atanmış olan tüm değerler çocukta da aynıdır.

Yığın (Stack) Segmenti: O ana kadar çağrılmış fonksiyonlar, bu fonksiyonların yerel (local) değişkenleri ve bu değişkenlerin fork() anındaki değerleri. fork()'tan önce tanımlanmış ve değer atanmış tüm yerel değişkenler çocukta da aynı değere sahiptir.

Heap Segmenti: malloc() gibi fonksiyonlarla dinamik olarak ayrılmış bellek bölgeleri ve içerikleri.

CPU Kayıtçıları (Registers): Program sayacı (PC - hangi komutun çalıştırılacağını gösterir), yığın işaretçisi (SP), çerçeve işaretçisi (FP/BP) ve diğer genel amaçlı kayıtçıların fork() anındaki değerleri. Çocuk işlem, ebeveynin kaldığı yerden devam edebilmek için aynı program sayacı değeriyle başlar (tek istisna dönüş değeri için kullanılan eax gibi kayıtçılardır).

Açık Dosya Tanımlayıcıları (File Descriptors): Ebeveynin fork() anında açık tuttuğu tüm dosyalar, soketler vb. çocukta da açıktır ve aynı dosya tablosu girişini (file table entry) işaret ederler. Bu, dosya konum işaretçisinin (file offset) paylaşılması anlamına gelir.

Diğer Özellikler: Çalışma dizini, sinyal ayarları, ortam değişkenleri vb.

Sonuç: Çocuk süreç, fork() çağrısının yapıldığı andaki ebeveyn sürecin tam bir kopyası olarak hayata başlar. Sanki zaman o noktada ikiye ayrılmış gibidir. Her iki süreç de fork() çağrısının hemen sonrasındaki komuttan çalışmaya devam eder, ancak artık bağımsızdırlar (CoW ile bellek ayrımı gerektiğinde yapılır).

Yani, fork()'tan önceki kodun çalışmasıyla oluşan tüm durum (değişken değerleri, açık dosyalar, programın konumu vb.) çocuğa miras kalır. Çocuk sadece fork()'tan sonraki kodla başlamaz, fork() anındaki tüm geçmişiyle birlikte başlar. Tek fark, fork()'un kendisinin dönüş değeridir.


----------------------





Peki, değerli öğrencilerim, fork() konusunu en baştan alıp, tüm detaylarıyla ve kafanızda hiçbir soru işareti kalmayacak şekilde toparlayalım. Bu, işletim sistemlerinin en zarif ve güçlü mekanizmalarından biridir.

(Tahtaya "fork(): Süreç Doğumu" yazar)

1. Temel Kavram: Süreç (Process) Nedir?

Öncelikle şunu hatırlayalım: Bir süreç, çalışan bir programdır. Kendi bellek alanına (kod, veri, yığın), kendi komut sayacına (nereyi çalıştırdığını takip eden), kendi açık dosya listesine ve kendi kimliğine (PID) sahip olan bağımsız bir varlıktır.

2. fork(): Bir Fonksiyon Değil, Bir Sistem Çağrısı

Kodumuzda pid_t id = fork(); yazdığımızda, bu sıradan bir C fonksiyonu çağrısı gibi görünse de, aslında çok daha derin bir işlem yapıyoruz. Bu bir sistem çağrısıdır (system call). Ne demek bu? Şu demek:

Programımız o an kullanıcı kipinde (user mode) çalışır. Bu kipte donanıma doğrudan erişim gibi hassas işlemleri yapamayız.

fork() çağrısıyla birlikte programımız işletim sisteminden özel bir istekte bulunur. Bu istek üzerine kontrol, işlemcinin daha yetkili olduğu çekirdek kipine (kernel mode) geçer.

Süreç yaratmak gibi temel ve kritik bir işlemi ancak işletim sisteminin çekirdeği (kernel) gerçekleştirebilir.

3. Çekirdeğin Büyülü Dünyası: fork() Anı

Kontrol çekirdeğe geçtiğinde neler oluyor?

Kopyalama Aşaması: Çekirdek, fork() çağrısını yapan süreci (buna artık ebeveyn - parent diyelim) alır ve onun neredeyse birebir kopyasını oluşturur.

Bellek Alanı: Ebeveynin kod, veri ve yığın (stack) segmentleri kopyalanır. Ancak! Modern sistemler burada akıllıca bir optimizasyon kullanır: Copy-on-Write (CoW - Yazma Anında Kopyala). Başlangıçta, bellek sayfaları fiziksel olarak kopyalanmaz, ebeveyn ve yeni süreç (buna da çocuk - child diyeceğiz) aynı sayfaları paylaşır. Ne zaman ki süreçlerden biri bu paylaşılan sayfaya yazmaya çalışır, işte o zaman o sayfa kopyalanır ve her sürecin kendi özel kopyası olur. Bu, fork()'u çok daha verimli hale getirir.

Diğer Kaynaklar: Açık dosya tanımlayıcıları (file descriptors), sinyal ayarları, ortam değişkenleri gibi birçok özellik de kopyalanır. Çocuk, ebeveynin bir klonudur ama artık kendi hayatı vardır.

Kimliklendirme: Çekirdek, bu yeni oluşturulan çocuk sürece, sistemdeki diğer tüm süreçlerden farklı, benzersiz bir Süreç Kimliği (PID) atar. Bu PID, ebeveynin PID'sinden farklıdır ve genellikle (ama kesinlikle kural değil!) daha büyük bir sayıdır, çünkü PID'ler genellikle artan sırada atanır (ta ki maksimuma ulaşıp başa dönene kadar).

Hazırlık: Hem ebeveyn hem de çocuk süreç, çalışmaya hazır (runnable) olarak işaretlenir ve işletim sisteminin zamanlayıcısının (scheduler) sıraya koymasını bekler.

4. O Kritik An: Farklılaşma ve Dönüş Değerleri (eax Kayıtçısı)

İşte en çok kafa karıştıran nokta: Nasıl oluyor da tek bir fork() satırı, iki farklı süreçte iki farklı değer döndürüyor? Cevap yine çekirdekte gizli.

Çekirdek, tüm kopyalama ve hazırlık işlerini bitirdikten sonra, kontrolü tekrar kullanıcı kipine iade etmeden hemen önce şunu yapar:

Ebeveyn Sürece Dönüş: Çekirdek, ebeveyn sürecin işlem bağlamına (context) geri dönerken, fonksiyon dönüş değerini tutmak için kullanılan özel bir işlemci kayıtçısına (CPU register) – x86 mimarisinde bu genellikle eax kayıtçısıdır – yeni oluşturulan çocuğun PID'sini yazar.

Çocuk Sürece Dönüş: Çekirdek, yeni oluşturulan çocuk sürecin işlem bağlamına geri dönerken, çocuğun kendi eax (veya eşdeğeri) kayıtçısına 0 (sıfır) değerini yazar.

Neden böyle?

Ebeveyn: Genellikle çocuğunu yönetmek (örneğin wait() ile bitmesini beklemek, kill() ile sinyal göndermek) isteyecektir. Bunu yapabilmesi için çocuğunun kimliğine, yani PID'sine ihtiyacı vardır. fork() bu bilgiyi ona doğrudan verir.

Çocuk: Kendi PID'sini zaten getpid() sistem çağrısıyla istediği zaman öğrenebilir. fork()'tan alması gereken en kritik bilgi, kendisinin yeni oluşturulan çocuk olduğudur. 0 değeri, bu ayrımı yapmanın en basit ve standart yoludur. Ebeveyninin kimliğini (PID) merak ederse, getppid() çağrısını kullanabilir.

5. Kullanıcı Kipine Dönüş ve Sonuç

Çekirdek bu değerleri eax'e (veya ilgili kayıtçıya) yerleştirdikten sonra kontrolü kullanıcı kipine geri verir:

Ebeveyn Süreç: fork() çağrısından dönen değeri (yani eax'teki çocuğun PID'sini) alır ve C kodundaki id değişkenine atar. (id = <çocuğun_PID'si>). Bu yüzden id > 0 olur.

Çocuk Süreç: fork() çağrısından dönen değeri (yani kendi eax'indeki 0'ı) alır ve kendi id değişkenine atar. (id = 0).

Özetle Öğrencilerim:

fork() sihirli bir değnek gibidir. Tek bir çağrı ile kendisinin bir kopyasını yaratır. Bu kopyalama işlemini ve iki sürece farklı kimlikler (dönüş değerleri) atanmasını sağlayan asıl güç işletim sistemi çekirdeğidir. Çekirdek, dönüş yapmadan hemen önce, her bir sürecin dönüş değeri kayıtçısına (eax gibi) bilinçli olarak farklı değerler yükler: Ebeveyne çocuğun PID'sini, çocuğa ise 0'ı.

Bu sayede fork()'tan sonra gelen kod, id değişkeninin değerine bakarak (if (id == 0) çocuktur, else if (id > 0) ebeveyndir) hangi yolu izleyeceğini bilir. İşte bu kadar basit ama bir o kadar da güçlü bir mekanizma!

Unutmayın:

fork() bir sistem çağrısıdır.

Çekirdek süreci kopyalar (CoW ile optimize edilmiş).

Çekirdek çocuğa yeni bir PID atar.

Çekirdek, ebeveyne çocuğun PID'sini, çocuğa 0 döndürür (kayıtçılar aracılığıyla).

Bu, süreçlerin kendilerini ayırt etmelerini sağlar.

Var mı bu konuda anlaşılmayan bir nokta?

(Gözlüklerini düzeltir, öğrencilerin sorularını bekler)
 */

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
		waitpid(-1, NULL, 0);
        printf("-----------\n");
        printf("ben parent processim\n");
        printf("child process pid is=%d\n", id);
        printf("parent process pid is=%d\n", getpid());
        printf("-----------\n");
    }


}

/*
int main(int argc, char **argv, char **env)
{
    char *args[] = {
        "/usr/bin/ls",
        "-la",
        NULL
    };

    perror("hii");
    execve(args[0], args, env);
}
*/


/*int main() {
    pid_t id = fork();

    if (id < 0) {
        perror("fork error");
        return 1;
    } else if (id == 0) {
        // --- Çocuk İşlem ---
        printf("ÇOCUK: fork() bana %d döndürdü.\n", id); // id burada 0 olacak
        printf("ÇOCUK: Benim GERÇEK PID'im %d.\n", getpid()); // Kendi PID'sini alır
        printf("ÇOCUK: Ebeveynimin PID'i %d.\n", getppid()); // Ebeveynin PID'sini alır
    } else {
        // --- Ebeveyn İşlem ---
        printf("EBEVEYN: fork() bana çocuğumun PID'i olan %d değerini döndürdü.\n", id); // id burada çocuğun PID'si olacak
        printf("EBEVEYN: Benim PID'im %d.\n", getpid()); // Kendi PID'sini alır
        wait(NULL); // Çocuğun bitmesini bekle
        printf("EBEVEYN: Çocuk işlemi tamamlandı.\n");
    }
    return 0;
}


/*int main()
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
