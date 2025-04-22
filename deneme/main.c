#include "cell.h"
#include <stdio.h>
#include <unistd.h>    // fork(), execve() için
#include <sys/types.h> // pid_t için
#include <sys/wait.h>  // wait() için
#include <stdlib.h>    // exit(), EXIT_SUCCESS, EXIT_FAILURE, perror için
#include <string.h>    // strcmp için (isteğe bağlı)

// `#include "cell.h"` yerine standart başlıkları kullanıyoruz

// İsteğe bağlı: PATH'te komut arayan basit bir yardımcı fonksiyon
// Gerçek bir shell için bu çok daha kapsamlı olmalıdır.
char *find_command_in_path(const char *cmd) {
    char *path_env = getenv("PATH"); // Ortam değişkenlerinden PATH'i al
    if (!path_env) return NULL; // PATH yoksa

    char *path_copy = strdup(path_env); // PATH'i değiştirmemek için kopyala
    if (!path_copy) return NULL;

    char *dir = strtok(path_copy, ":"); // ':' ayıracı ile dizinleri böl

    while (dir != NULL) {
        // Dizin yolu + '/' + komut adı için yeterli yer ayır
        size_t fullpath_len = strlen(dir) + 1 + strlen(cmd) + 1;
        char *full_path = malloc(fullpath_len);
        if (!full_path) {
            free(path_copy);
            return NULL;
        }

        sprintf(full_path, "%s/%s", dir, cmd);

        // Dosyanın var olup olmadığını ve çalıştırılabilir olup olmadığını kontrol et
        // access(full_path, X_OK) fonksiyonu daha iyidir
        if (access(full_path, X_OK) == 0) { // Çalıştırılabilirse
            free(path_copy);
            return full_path; // Tam yolu döndür
        }

        free(full_path); // Bulunamadı, bellekten sil
        dir = strtok(NULL, ":"); // Sonraki dizine geç
    }

    free(path_copy); // PATH kopyasını sil
    return NULL; // Komut bulunamadı
}


int main(int argc, char **argv, char **env) // env argümanı artık kullanılıyor
{
    int status;
    pid_t pid;
    char *command_path = NULL; // Çalıştırılacak komutun tam yolu

    if (argc < 2) {
        fprintf(stderr, "Kullanım: %s <komut> [argumanlar...]\n", argv[0]);
        return 1; // Hata kodu döndür
    }

    // Kullanıcı 'exit' yazdıysa yerleşik komut gibi işle (basit bir örnek)
    if (strcmp(argv[1], "exit") == 0) {
         printf("Shell'den çıkılıyor.\n");
         exit(0); // Shell'den çık
    }
    // Başka yerleşik komutları da buraya ekleyebilirsiniz (cd, export vb.)
    // ...

    // Komutun tam yolunu bulmaya çalış
    command_path = find_command_in_path(argv[1]);

    // Eğer tam yol bulunamadıysa ve komut './komut' gibi bir şey değilse
    // veya tam yol olarak verilmişse (bu durumda find_command_in_path işe yaramazdı,
    // bu kontrol gerçek shell'de daha karmaşıktır)
    // Şimdilik sadece find_command_in_path başarılı olmadıysa hata verelim.
    // Gerçek bir shell'de "./komut" veya "/bin/ls" gibi durumları da kontrol etmeniz gerekir.

    if (command_path == NULL) {
        // Komut PATH'te bulunamadı
        fprintf(stderr, "Komut bulunamadı: %s\n", argv[1]);
        return 1; // Hata
    }


    pid = fork(); // Süreç oluştur

    if (pid < 0) {
        // fork hatası
        perror("fork hatası");
        free(command_path); // Ayrılan belleği serbest bırak
        return 1; // Hata
    }

    if (pid == 0) { // Çocuk Süreç
        // execve(pathname, argv, envp)
        // command_path: find_command_in_path ile bulunan tam yol
        // argv + 1: Yeni programa geçirilecek argüman dizisi (argv[1] sonrası)
        // env: main fonksiyonuna gelen ortam değişkenleri dizisi
        execve(command_path, argv + 1, env);

        // Eğer buraya geldiysek, execve başarısız olmuştur.
        // Başarısızlık genellikle komutun çalıştırılamaması, izin olmaması vb. durumlarındadır.
        perror("execve hatası"); // execve neden başarısız oldu
        free(command_path); // Ayrılan belleği serbest bırak (sadece çocukta)
        exit(EXIT_FAILURE);     // Çocuk hata ile sonlanır
    } else { // Ebeveyn Süreç
        // Ebeveyn waitpid kullanarak çocuğun bitmesini bekler
        // waitpid(pid, &status, 0); -> Belirli bir pid'deki çocuğu bekle
        // wait(NULL); -> Herhangi bir çocuğu bekle (NULL durumuyla ilgilenme)
        wait(NULL); // Basitlik için herhangi bir çocuğu bekle

        // command_path sadece çocuk execve içinde kullanıldığı için parent'ta free edilebilir
        // veya execve sonrası free edilmesi (child'da) önemlidir.
        // Parent'ta free etmek için buraya taşıyabilirsiniz: free(command_path);

        printf("Çocuk süreç sonlandı.\n"); // Çocuğun bittiğini belirt

        // Ebeveyn normal akışına devam eder
    }

    // find_command_in_path tarafından ayrılan bellek parent'ta da serbest bırakılmalı
    // (execve başarılı olursa child bu satıra gelmez, başarısız olursa child kendi free eder)
    // Bu biraz karmaşık olabilir, basitlik için find_command_in_path'in döndürdüğü bellek yönetimine dikkat edin.
    // Eğer find_command_in_path NULL döndürmediyse, burada free(command_path); yapmalısınız.
    if (command_path != NULL) {
         free(command_path); // Ebeveynde ayrılan belleği serbest bırak
    }


    return (0); // Ebeveyn başarıyla sonlanır
}
/*
int main(int argc, char **argv, char **env)
{

	if (fork() == 0)
		execve(argv[1], argv + 1, env);

	wait(NULL);

	return (0);
}*/
