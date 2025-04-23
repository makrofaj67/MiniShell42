#include "cell.h"
#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit, EXIT_FAILURE, EXIT_SUCCESS, free, malloc
#include <string.h>     // strcmp, strtok
#include <unistd.h>     // fork, execve, getenv
#include <sys/wait.h>   // wait, waitpid
#include <sys/types.h>  // pid_t
#include <readline/readline.h>
#include <readline/history.h> // add_history için

// --- Yardımcı Fonksiyon (Basit Ayrıştırma) ---
// Komut satırını argümanlara böler.
// Daha karmaşık durumları (tırnaklar, boşluklar vb.) ele almaz.
// Döndürülen dizi free edilmelidir!
char **split_line(char *line) {
    int bufsize = 64;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        perror("split_line: Bellek ayırma hatası");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n\a"); // Boşluk, tab, enter, vs. ile böl

    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            // Eğer yeterli yer yoksa, diziyi genişlet
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                 perror("split_line: Bellek yeniden ayırma hatası");
                 exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL; // Argüman dizisini NULL ile sonlandır
    return tokens;
}

// --- Yardımcı Fonksiyon (Argüman Dizisini Temizleme) ---
void free_tokens(char **tokens) {
    // split_line fonksiyonu sadece line stringini böldüğü için
    // token'lar line'ın içindedir. Sadece diziyi free etmemiz yeterli.
    // Eğer token'lar için de malloc kullanılsaydı, her token'ı da free etmemiz gerekirdi.
    free(tokens);
}

// --- Komut Yürütme Fonksiyonu ---
// Dış komutları fork/execve ile çalıştırır
int execute_command(char **args, char **env) {
    pid_t pid, wpid;
    int status;

    if (args[0] == NULL) {
        // Boş komut girildi
        return 1; // Başarıyı temsil edebilir (shell devam eder)
    }

    // --- Yerleşik Komutlar ---
    if (strcmp(args[0], "exit") == 0) {
        printf("Shell'den çıkılıyor.\n");
        return 0; // Shell'den çıkmak için 0 döndür (main döngüsünde kontrol edilecek)
    }
    // Diğer yerleşik komutlar buraya eklenebilir (cd, echo, vb.)
    // else if (strcmp(args[0], "cd") == 0) {
    //    // cd komutu mantığı
    //    return 1;
    // }


    // --- Dış Komut Yürütme (fork + execve + wait) ---
    pid = fork(); // Yeni süreç oluştur

    if (pid < 0) {
        // Fork hatası
        perror("shell: fork hatası");
        return 1; // Hata kodu
    }

    if (pid == 0) { // Çocuk Süreç
        // args[0] çalıştırılacak komutun TAM YOLU olmalı (execve gereksinimi)
        // args: execve'ye geçirilecek argüman dizisi (ilk eleman komutun kendisi)
        // env: main'den gelen ortam değişkenleri
        execve(args[0], args, env);

        // Eğer execve geri döndüyse, hata olmuştur
        perror("shell: execve hatası");
        // execve başarısız olursa çocuk hata ile çıkmalı
        exit(EXIT_FAILURE);
    } else { // Ebeveyn Süreç
        // Çocuğun bitmesini bekle
        // waitpid: belirli bir süreci (pid) bekle
        // &status: çocuğun çıkış durumunu yazılacağı yer
        // 0: seçenekler (varsayılan)
        wpid = waitpid(pid, &status, 0); // wait(NULL) da kullanabilirsiniz

        if (wpid == -1) {
            perror("shell: waitpid hatası");
        }
        // Çocuğun çıkış durumu status değişkenindedir, WIFEXITED, WEXITSTATUS gibi
        // makrolarla kontrol edilebilir, ancak bu basit örnekte yapmıyoruz.
    }

    return 1; // Shell döngüsünün devam etmesi için 1 döndür
}

// --- Ana Shell Döngüsü ---
int main(int argc, char **argv, char **env) // env argümanı execve için alınır
{
    char *line;          // readline'dan gelen girdi stringi
    char **args;         // Ayrıştırılmış argüman dizisi
    int shell_status;    // Shell'in devam edip etmeyeceğini belirler (1=devam, 0=çıkış)

    shell_status = 1; // Başlangıçta shell çalışmaya devam etsin

    // Shell döngüsü: komut al, ayrıştır, çalıştır
    while (shell_status) {
        // Komut istemini göster ve kullanıcıdan girdi al
        line = readline("minishell> ");

        // Eğer readline NULL dönerse (EOF - Ctrl+D veya hata) shell'den çık
        if (line == NULL) {
            printf("exit\n"); // Bash gibi davran
            break;
        }

        // Boş satırları (sadece Enter) atla
        if (line[0] == '\0') {
            free(line); // readline tarafından ayrılan belleği serbest bırak
            continue; // Döngünün başına dön
        }

        // Geçmişe ekle
        add_history(line);

        // Komut satırını argümanlara ayır
        args = split_line(line);

        // Ayrıştırma başarılıysa komutu çalıştır
        if (args != NULL) {
             // execute_command fonksiyonu shell_status'ü etkileyebilir (örn. exit komutu)
             shell_status = execute_command(args, env);
        } else {
            // Ayrıştırma hatası (split_line'da bellek ayırma hatası gibi)
             shell_status = 0; // Shell'den çık
        }


        // readline ve split_line tarafından ayrılan belleği temizle
        free(line);
        if (args != NULL) {
            free_tokens(args); // split_line sadece token dizisini malloc/realloc yapar
        }
    }

    // Program sonu
    // Readline geçmişini kaydetme vb. burada yapılabilir (isteğe bağlı)

    return 0;
}


