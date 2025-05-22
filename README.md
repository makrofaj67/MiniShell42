# MiniShell42 Projesi

## Executor ve Dahili Komutlar Kılavuzu

Bu belge, MiniShell42 projesinin executor (yürütücü) ve dahili komutlar (builtins) bileşenlerinin nasıl çalıştığını detaylı bir şekilde açıklamaktadır.

![MiniShell Logo](https://upload.wikimedia.org/wikipedia/commons/thumb/3/35/Tux.svg/150px-Tux.svg.png)

## İçindekiler

1. [Genel Yapı](#genel-yapı)
    - [Proje Mimarisi](#proje-mimarisi)
    - [Veri Akışı](#veri-akışı)
2. [Executor Modülü](#executor-modülü)
   - [AST İşleme](#ast-i̇şleme)
   - [Komut Yürütme](#komut-yürütme)
   - [Pipe İşleme](#pipe-i̇şleme)
   - [Yönlendirmeler](#yönlendirmeler)
   - [Harici Komutlar](#harici-komutlar)
3. [Dahili Komutlar](#dahili-komutlar)
   - [echo](#echo)
   - [cd](#cd)
   - [pwd](#pwd)
   - [export](#export)
   - [unset](#unset)
   - [env](#env)
   - [exit](#exit)
4. [Sinyal İşleme](#sinyal-i̇şleme)
5. [Ortam Değişkenleri Yönetimi](#ortam-değişkenleri-yönetimi)
6. [Hata İşleme](#hata-i̇şleme)
7. [Özel Durumlar](#özel-durumlar)
8. [Performans Değerlendirmesi](#performans-değerlendirmesi)
9. [Kaynaklar ve Referanslar](#kaynaklar-ve-referanslar)

## Genel Yapı

MiniShell42, UNIX kabuk fonksiyonelliğini taklit eden bir C programıdır. Bu proje, kullanıcının komut satırı arayüzü aracılığıyla komutlar girmesine ve bu komutların işletim sistemi tarafından yürütülmesine olanak tanır.

### Proje Mimarisi

Proje, modüler bir mimari ile tasarlanmıştır ve aşağıdaki ana bileşenlerden oluşur:

```
MiniShell42
├── Command Parser (Komut Ayrıştırıcı)
├── Lexer (Sözcük Çözümleyici)
├── Parser (Ayrıştırıcı)
├── Executor (Yürütücü)
│   ├── Builtin Commands (Dahili Komutlar)
│   └── External Commands (Harici Komutlar)
└── Environment Manager (Ortam Yöneticisi)
```

### Veri Akışı

Komut yürütme süreci şu adımlardan oluşur:

1. **Kullanıcı Girdisi**: Readline kütüphanesi ile kullanıcı girdisi alınır.
2. **Sözcük Çözümleme (Lexing)**: Girdi, lexer tarafından token'lara ayrılır.
3. **Ayrıştırma (Parsing)**: Token'lar, parser tarafından Abstract Syntax Tree (AST) yapısına dönüştürülür.
4. **Yürütme (Execution)**: AST, executor tarafından işlenir ve komutlar çalıştırılır.
5. **Çıktı**: Komutun çıktısı kullanıcıya döndürülür ve yeni bir komut beklenir.

```
Kullanıcı Girdisi → Lexer → Parser → Executor → Çıktı
```

## Executor Modülü

Executor modülü, AST yapısını işleyerek komutların çalıştırılmasından sorumludur. Bu modül, `/src/d_executor/` dizininde bulunur ve aşağıdaki dosyalardan oluşur:

| Dosya | Açıklama |
|-------|----------|
| `a_execute_ast.c` | Ana yürütme fonksiyonlarını içerir |
| `b_execute_external.c` | Harici komutları çalıştırmak için yardımcı fonksiyonlar |
| `c_execute_pipe.c` | Pipe işleme fonksiyonları |
| `d_external_command.c` | Harici komutlar için ana yürütücü |
| `e_redirections.c` | Giriş/çıkış yönlendirme fonksiyonları |
| `f_heredoc.c` | Heredoc yönlendirmesi için özel işleme |

### AST İşleme

AST (Abstract Syntax Tree - Soyut Sözdizimi Ağacı), komutların hiyerarşik bir yapıda temsil edilmesini sağlar. Executor, bu ağacı dolaşarak her düğümü uygun şekilde işler.

AST işleme, `execute_ast` fonksiyonu ile başlar. Bu fonksiyon, AST'nin kök düğümünü alır ve düğüm türüne göre işler:

```c
int execute_ast(ast_node *root, t_variable_list *env_list, int *exit_status)
{
    t_executor_data data;
    
    if (!root)
        return (0);
    data.env_list = env_list;
    data.last_exit_status = *exit_status;
    data.last_pid = 0;
    if (root->type == COMMAND_NODE)
        *exit_status = execute_command_node(root, &data);
    else if (root->type == PIPE_NODE)
        *exit_status = execute_pipe_node(root, &data);
    return (*exit_status);
}
```

#### AST Veri Yapısı

AST'de iki tür düğüm bulunur:
1. **COMMAND_NODE**: Basit bir komutu temsil eder (örn. `ls -la`)
2. **PIPE_NODE**: İki komut arasındaki pipe operasyonunu temsil eder (örn. `ls | grep .c`)

Her düğüm şu yapıdadır:

```c
typedef struct t_ast_node
{
    ast_type            type;           // Düğüm türü (COMMAND_NODE veya PIPE_NODE)
    struct t_ast_node   *left;          // Sol alt ağaç
    struct t_ast_node   *right;         // Sağ alt ağaç
    command_value       *value;         // Komut değeri (sadece COMMAND_NODE için)
}   ast_node;
```

### Komut Yürütme

Komut düğümleri, `execute_command_node` fonksiyonu ile işlenir. Bu fonksiyon, komutun yönlendirmelerini işler, komutu çalıştırır ve orijinal dosya tanımlayıcılarını geri yükler.

```c
int execute_command_node(ast_node *node, t_executor_data *data)
{
    int status;
    
    if (!node || !node->value)
        return (1);
    if (handle_redirections(node->value, data) != 0)
        return (1);
    status = execute_simple_command(node->value, data);
    restore_redirections(node->value);
    return (status);
}
```

#### Komut Yürütme Süreci

1. **Yönlendirmeleri İşle**: Komutun tüm yönlendirmeleri (input, output, append, heredoc) işlenir
2. **Komutu Çalıştır**: Dahili bir komut ise doğrudan çalıştırılır, değilse harici komut olarak işlenir
3. **Orijinal Dosya Tanımlayıcılarını Geri Yükle**: Yönlendirme işlemlerinden sonra orijinal stdin ve stdout geri yüklenir

#### Komut Değeri Yapısı

Her komut, aşağıdaki veri yapısı ile temsil edilir:

```c
typedef struct t_command_value
{
    char            **arg_array;        // Komut argümanları
    t_redirection   **redirections;     // Yönlendirmeler
    int             stdin_backup;       // Orijinal stdin yedeklemesi
    int             stdout_backup;      // Orijinal stdout yedeklemesi
}   command_value;
```

### Pipe İşleme

Pipe işleme, bir komutun çıktısını diğer bir komutun girdisi olarak kullanma mekanizmasıdır. MiniShell42'de bu işlem, `execute_pipe_node` fonksiyonu tarafından gerçekleştirilir.

```c
int execute_pipe_node(ast_node *node, t_executor_data *data)
{
    int pipefd[2];
    int status;
    
    if (!node || !node->left || !node->right)
        return (1);
    if (pipe(pipefd) == -1)
        return (1);
    // Sol ve sağ çocuklar için fork işlemleri burada
    // ...
    return (status);
}
```

#### Pipe İşlem Aşamaları

1. **Pipe Oluşturma**: İki süreç arasında veri akışı için bir pipe oluşturulur
2. **Sol Komut Yürütme**: İlk çocuk süreç oluşturulur ve sol komut çalıştırılır, çıktısı pipe'a yazılır
3. **Sağ Komut Yürütme**: İkinci çocuk süreç oluşturulur ve sağ komut çalıştırılır, girdisi pipe'dan okunur
4. **Senkronizasyon**: Ana süreç, çocuk süreçlerin tamamlanmasını bekler

#### Örnek Pipe İşlemi

Örneğin, `ls -la | grep .c` komutu için:

```
          PIPE_NODE
          /       \
COMMAND_NODE       COMMAND_NODE
   (ls -la)           (grep .c)
```

1. `ls -la` komutu çalıştırılır ve çıktısı pipe'a yazılır
2. `grep .c` komutu çalıştırılır ve girdisini pipe'dan okur
3. Ana süreç, her iki komutun da tamamlanmasını bekler

### Yönlendirmeler

MiniShell42, dört farklı yönlendirme türünü destekler:

| Yönlendirme | Sembol | Açıklama |
|-------------|--------|----------|
| Input Redirection | `<` | Bir dosyadan girdi alma |
| Output Redirection | `>` | Bir dosyaya çıktı yazma (dosya yoksa oluşturulur, varsa üzerine yazılır) |
| Append Redirection | `>>` | Bir dosyaya çıktı ekleme (dosya yoksa oluşturulur, varsa sonuna eklenir) |
| Heredoc | `<<` | Belirtilen sınırlayıcıya kadar girdi alma |

Yönlendirmeler, `handle_redirections` fonksiyonu tarafından işlenir:

```c
int handle_redirections(command_value *cmd, t_executor_data *data)
{
    int i;
    int ret;
    
    if (!cmd || !cmd->redirections)
        return (0);
    cmd->stdin_backup = dup(STDIN_FILENO);
    cmd->stdout_backup = dup(STDOUT_FILENO);
    i = 0;
    while (cmd->redirections[i])
    {
        if (cmd->redirections[i]->type == REDIR_IN)
            ret = handle_input_redirection(cmd->redirections[i]);
        else if (cmd->redirections[i]->type == REDIR_OUT)
            ret = handle_output_redirection(cmd->redirections[i]);
        else if (cmd->redirections[i]->type == REDIR_APPEND)
            ret = handle_append_redirection(cmd->redirections[i]);
        else if (cmd->redirections[i]->type == REDIR_HEREDOC)
            ret = handle_heredoc(cmd->redirections[i]);
        if (ret != 0)
            return (ret);
        i++;
    }
    return (0);
}
```

#### Yönlendirme Veri Yapısı

Yönlendirmeler, aşağıdaki veri yapısı ile temsil edilir:

```c
typedef struct s_redirection
{
    t_redir_type    type;       // Yönlendirme türü
    char            *filename;  // Yönlendirme dosyası/sınırlayıcısı
}   t_redirection;
```

#### Yönlendirme Örneği

Örneğin, `ls > output.txt` komutu için:
1. Orijinal stdout yedeklenir
2. `output.txt` dosyası oluşturulur veya üzerine yazılır
3. stdout, `output.txt` dosyasına yönlendirilir
4. `ls` komutu çalıştırılır ve çıktısı `output.txt` dosyasına yazılır
5. Orijinal stdout geri yüklenir

#### Heredoc İşleme

Heredoc, `<<` yönlendirmesi ile kullanılır ve belirtilen sınırlayıcıya kadar girdi alınmasını sağlar. Örneğin:

```bash
cat << EOF
Bu bir heredoc örneğidir.
Birden fazla satır içerebilir.
EOF
```

Bu işlem, `handle_heredoc` fonksiyonu tarafından gerçekleştirilir:

```c
int handle_heredoc(t_redirection *redir)
{
    int pipe_fd[2];
    
    if (pipe(pipe_fd) == -1)
        return (1);
    write_heredoc_content(pipe_fd[1], redir->filename);
    close(pipe_fd[1]);
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[0]);
    return (0);
}
```

### Harici Komutlar

MiniShell42, dahili komutlar dışındaki tüm komutları harici komut olarak değerlendirir ve bunları çalıştırmak için `execve` sistem çağrısını kullanır. Bu işlem, `execute_external_command` fonksiyonu tarafından gerçekleştirilir:

```c
int execute_external_command(command_value *cmd, t_executor_data *data)
{
    pid_t pid;
    int status;
    
    pid = fork();
    if (pid == -1)
        return (1);
    if (pid == 0)
        child_process(cmd, data);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (1);
}
```

#### Harici Komut Yürütme Süreci

1. **Çocuk Süreç Oluşturma**: `fork()` ile yeni bir süreç oluşturulur
2. **Komut Yolunu Bulma**: `find_command_path` fonksiyonu ile komutun tam yolu `PATH` değişkeninden bulunur
3. **Ortam Değişkenlerini Hazırlama**: `create_env_array` fonksiyonu ile ortam değişkenleri bir dizi olarak hazırlanır
4. **Komutu Çalıştırma**: `execve` sistem çağrısı ile komut çalıştırılır
5. **Çıkış Durumunu Yakalama**: Ana süreç, çocuk sürecin tamamlanmasını bekler ve çıkış durumunu alır

#### Komut Yolunu Bulma

Komutun tam yolu, `find_command_path` fonksiyonu ile bulunur:

```c
char *find_command_path(char *cmd, t_variable_list *env_list)
{
    char            *path_env;
    char            **paths;
    char            *cmd_path;
    int             i;
    struct stat     st;
    
    if (!cmd || !*cmd)
        return (NULL);
    if (cmd[0] == '/' || cmd[0] == '.')
        return (ft_strdup(cmd));
    path_env = get_variable_value(env_list, "PATH");
    if (!path_env)
        return (NULL);
    paths = ft_split(path_env, ':');
    // PATH içindeki her dizini kontrol et
    // ...
    return (cmd_path);
}
```

Bu fonksiyon:
1. Komut mutlak veya göreli bir yol ise (`/` veya `.` ile başlıyorsa) doğrudan kullanır
2. Değilse, `PATH` ortam değişkeninden olası dizinleri alır
3. Her dizinde komutun varlığını kontrol eder
4. Komut bulunursa tam yolunu döndürür, bulunamazsa `NULL` döndürür

#### Çalıştırılabilirlik Kontrolü

Bir dosyanın çalıştırılabilir olup olmadığı, `stat` sistem çağrısı ile kontrol edilir:

```c
if (stat(cmd_path, &st) == 0 && (st.st_mode & S_IXUSR))
```

Bu kontrol, dosyanın mevcut olduğunu ve kullanıcı için çalıştırma iznine sahip olduğunu doğrular.

## Dahili Komutlar

MiniShell42, POSIX standardına uygun olarak aşağıdaki dahili komutları destekler:

| Komut | Açıklama | Seçenekler |
|-------|----------|------------|
| echo | Argümanları standart çıktıya yazdırır | -n (yeni satır ekleme) |
| cd | Çalışma dizinini değiştirir | - |
| pwd | Mevcut çalışma dizinini yazdırır | - |
| export | Ortam değişkenlerini ayarlar ve dışa aktarır | - |
| unset | Ortam değişkenlerini kaldırır | - |
| env | Tüm ortam değişkenlerini listeler | - |
| exit | Kabuğu belirtilen çıkış koduyla sonlandırır | - |

Dahili komutlar, `/src/e_builtins/` dizininde uygulanmıştır:

- `a_echo_cd.c`: `echo` ve `cd` komutları
- `b_pwd_export.c`: `pwd` ve `export` komutları
- `c_unset_env.c`: `unset` ve `env` komutları
- `d_exit.c`: `exit` komutu

### echo

`echo` komutu, argümanları standart çıktıya yazdırır. `-n` seçeneği ile çağrıldığında, çıktının sonuna yeni satır eklenmez.

```c
int ft_echo(char **args)
{
    int i;
    int n_flag;
    
    n_flag = 0;
    i = 0;
    if (args && args[0] && ft_strcmp(args[0], "-n") == 0) {
        n_flag = 1;
        i = 1;
    }
    while (args && args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (!n_flag)
        printf("\n");
    return (0);
}
```

#### echo Kullanım Örnekleri

```bash
# Normal kullanım
echo Merhaba Dünya     # Çıktı: Merhaba Dünya (yeni satır ile)

# -n seçeneği ile kullanım
echo -n Merhaba Dünya  # Çıktı: Merhaba Dünya (yeni satır olmadan)

# Ortam değişkenleri ile kullanım
echo $HOME             # Çıktı: /home/kullanici (HOME değişkeninin değeri)
```

### cd

`cd` komutu, çalışma dizinini değiştirir. Argüman verilmezse, kullanıcının ana dizinine (`HOME`) geçer.

```c
int ft_cd(char **args, t_variable_list *env_list)
{
    char *path;
    char *old_pwd;
    char cwd[PATH_MAX];
    
    if (!args || !args[0]) {
        path = get_variable_value(env_list, "HOME");
        if (!path) {
            fprintf(stderr, "minishell: cd: HOME not set\n");
            return (1);
        }
    }
    else
        path = args[0];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return (1);
    old_pwd = ft_strdup(cwd);
    if (chdir(path) != 0) {
        fprintf(stderr, "minishell: cd: %s: %s\n", path, strerror(errno));
        free(old_pwd);
        return (1);
    }
    update_pwd_vars(env_list, old_pwd);
    free(old_pwd);
    return (0);
}
```

#### cd Fonksiyonunun Aşamaları

1. **Hedef Dizini Belirleme**: Argüman verilmişse hedef dizin olarak kullanılır, verilmemişse `HOME` değişkeni kullanılır
2. **Mevcut Dizini Alma**: `getcwd` ile mevcut çalışma dizini alınır
3. **Dizin Değiştirme**: `chdir` ile yeni dizine geçilir
4. **Ortam Değişkenlerini Güncelleme**: `PWD` ve `OLDPWD` değişkenleri güncellenir

#### cd Kullanım Örnekleri

```bash
# Ana dizine git
cd               # HOME değişkeninin değerine gider

# Mutlak yol ile dizin değiştirme
cd /usr/local    # /usr/local dizinine gider

# Göreli yol ile dizin değiştirme
cd ../bin        # Üst dizinin bin alt dizinine gider
```

### pwd

`pwd` komutu, mevcut çalışma dizinini yazdırır.

```c
int ft_pwd(void)
{
    char cwd[PATH_MAX];
    
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        fprintf(stderr, "minishell: pwd: %s\n", strerror(errno));
        return (1);
    }
    printf("%s\n", cwd);
    return (0);
}
```

Bu fonksiyon, `getcwd` sistem çağrısını kullanarak mevcut çalışma dizinini alır ve standart çıktıya yazdırır.

### export

`export` komutu, ortam değişkenlerini ayarlar ve dışa aktarır. Argüman verilmezse, tüm dışa aktarılan değişkenleri alfabetik sırayla listeler.

```c
int ft_export(char **args, t_variable_list *env_list)
{
    int i;
    int ret;
    
    ret = 0;
    if (!args || !args[0]) {
        print_exported_vars(env_list);
        return (0);
    }
    i = 0;
    while (args[i]) {
        if (handle_export_arg(args[i], env_list) != 0)
            ret = 1;
        i++;
    }
    return (ret);
}
```

#### export Fonksiyonunun Aşamaları

1. **Argüman Kontrolü**: Argüman verilmemişse, tüm dışa aktarılan değişkenler listelenir
2. **Argüman İşleme**: Her argüman için `handle_export_arg` fonksiyonu çağrılır
3. **Değişken Adı Geçerliliği**: Değişken adının geçerli bir tanımlayıcı olup olmadığı kontrol edilir
4. **Değişken Ayarlama**: Geçerli ise, değişken ayarlanır ve dışa aktarılır

#### export Kullanım Örnekleri

```bash
# Değişken ayarlama
export VAR=deger           # VAR değişkenini "deger" olarak ayarlar ve dışa aktarır

# Çoklu değişken ayarlama
export VAR1=deger1 VAR2=deger2

# Mevcut dışa aktarılan değişkenleri listeleme
export                      # Tüm dışa aktarılan değişkenleri listeler
```

### unset

`unset` komutu, ortam değişkenlerini kaldırır.

```c
int ft_unset(char **args, t_variable_list *env_list)
{
    int i;
    int ret;
    
    ret = 0;
    if (!args || !args[0])
        return (0);
    i = 0;
    while (args[i]) {
        if (!is_valid_identifier(args[i])) {
            fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n",
                args[i]);
            ret = 1;
        }
        else
            unset_variable(env_list, args[i]);
        i++;
    }
    return (ret);
}
```

#### unset Fonksiyonunun Aşamaları

1. **Argüman Kontrolü**: Argüman verilmemişse, hiçbir şey yapılmaz
2. **Değişken Adı Geçerliliği**: Her argüman için değişken adının geçerli olup olmadığı kontrol edilir
3. **Değişken Kaldırma**: Geçerli ise, değişken ortamdan kaldırılır

#### unset Kullanım Örnekleri

```bash
# Tek değişken kaldırma
unset VAR              # VAR değişkenini ortamdan kaldırır

# Çoklu değişken kaldırma
unset VAR1 VAR2 VAR3   # VAR1, VAR2 ve VAR3 değişkenlerini kaldırır
```

### env

`env` komutu, tüm ortam değişkenlerini listeler.

```c
int ft_env(t_variable_list *env_list)
{
    t_variable_node *current;
    
    if (!env_list || !env_list->head)
        return (0);
    current = env_list->head;
    while (current) {
        if (current->is_exported)
            printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
    return (0);
}
```

Bu fonksiyon, ortam listesindeki tüm dışa aktarılan değişkenleri dolaşır ve her birini `key=value` formatında yazdırır.

### exit

`exit` komutu, kabuğu belirtilen çıkış koduyla sonlandırır.

```c
int ft_exit(char **args, int *exit_status)
{
    int status;
    
    printf("exit\n");
    if (!args || !args[0]) {
        exit(*exit_status);
        return (0);
    }
    if (!is_numeric(args[0])) {
        fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
            args[0]);
        exit(255);
    }
    status = str_to_exit_code(args[0]);
    if (args[1]) {
        fprintf(stderr, "minishell: exit: too many arguments\n");
        *exit_status = 1;
        return (1);
    }
    exit(status);
    return (0);
}
```

#### exit Fonksiyonunun Aşamaları

1. **Argüman Kontrolü**: Argüman verilmemişse, son çıkış kodu ile çıkılır
2. **Sayısal Argüman Kontrolü**: Argümanın sayısal olup olmadığı kontrol edilir
3. **Çoklu Argüman Kontrolü**: Birden fazla argüman verilmişse hata döndürülür
4. **Çıkış**: Belirtilen çıkış kodu ile programdan çıkılır

#### exit Kullanım Örnekleri

```bash
# Argümansız kullanım
exit              # Son komutun çıkış kodu ile çıkar

# Çıkış kodu ile kullanım
exit 42           # 42 çıkış kodu ile çıkar

# Hatalı kullanım
exit abc          # Hata: sayısal argüman gerekli
exit 1 2 3        # Hata: çok fazla argüman
```

## Sinyal İşleme

MiniShell, SIGINT (Ctrl+C) ve SIGQUIT (Ctrl+\\) sinyallerini işler. Sinyal işleme, kabuk durumuna göre farklılık gösterir:

- Etkileşimli mod: SIGINT yeni bir satır yazdırır, SIGQUIT yok sayılır
- Yürütme modu: Sinyaller varsayılan davranışlarını korur
- Heredoc modu: SIGINT heredoc'u sonlandırır, SIGQUIT yok sayılır

```c
void setup_interactive_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;
    
    sa_int.sa_handler = sig_int_handler;
    sa_int.sa_flags = SA_RESTART;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
    
    sa_quit.sa_handler = SIG_IGN;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}
```

---

Bu README, MiniShell42 projesinin executor ve dahili komut bileşenlerinin genel bir bakışını sağlar. Daha fazla bilgi için kaynak kodunu inceleyebilirsiniz.