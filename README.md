# MiniShell42 Builtins İyileştirme Raporu

Bu döküman, MiniShell42 projesinin `f_builtins` klasöründeki builtin komut uygulamalarında tespit edilen sorunları ve çözüm önerilerini içermektedir.

## 1. unset_cmd Fonksiyonundaki Sorunlar

**Dosya:** `src/f_builtins/unset.c`

### Mevcut Sorunlar:

1. **Segmentation Fault Riski**: `tmp->next->key` erişimi sırasında `tmp->next` NULL olabilir.
2. **Mantık Hatası**: If-else bloğunda, `else` durumunda `return` var, ancak bu durumda döngü her zaman sonlanacaktır. Hiçbir zaman `tmp = tmp->next` satırı çalışmaz.
3. **Liste Başı Kontrolü Yok**: Listenin ilk elemanı silinmek istenirse ele alınmıyor.

### Çözüm Önerisi:

```c
void unset_cmd(char *args, t_env **env)
{
    t_env *tmp;
    t_env *to_delete;

    if (!env || !*env || !args)
        return;
        
    // Listenin başını kontrol et
    if ((*env) && mini_strcmp_path((*env)->key, args) == 1)
    {
        to_delete = *env;
        *env = (*env)->next;
        free(to_delete->key);
        free(to_delete->value);
        free(to_delete);
        return;
    }
    
    tmp = *env;
    while (tmp && tmp->next)
    {
        if (mini_strcmp_path(tmp->next->key, args) == 1)
        {
            to_delete = tmp->next;
            tmp->next = to_delete->next;
            free(to_delete->key);
            free(to_delete->value);
            free(to_delete);
            return;
        }
        tmp = tmp->next;
    }
}
```

## 2. builtin_selector Fonksiyonundaki Sorunlar

**Dosya:** `src/f_builtins/selector.c`

### Mevcut Sorunlar:

1. **Hata Kontrolü Eksikliği**: `mini_strcmp_path` fonksiyonu çağrılarında bazı yerlerde dönüş değerinin 1 olması kontrolü eksik (bazı yerlerde == 1 var, bazı yerlerde yok).
2. **Çıktı Değeri Belirsizliği**: Son `else` durumunda `return` ifadesi kullanılmazsa, fonksiyon belirsiz bir değer döndürebilir.
3. **Argüman Kontrolü Yok**: Bazı fonksiyon çağrılarında (örneğin `unset_cmd`, `export_cmd`) NULL argüman kontrolü eksik.

### Çözüm Önerisi:

```c
int builtin_selector(char **args, t_env **env, t_env **env_var)
{
    if (!args || !args[0])
        return (1);

    if (mini_strcmp_path(args[0], "pwd") == 1)
        pwd_cmd();
    else if (mini_strcmp_path(args[0], "clear") == 1)
        printf("\033[H\033[2J");
    else if (mini_strcmp_path(args[0], "exit") == 1)
        exit(EXIT_SUCCESS);
    else if (mini_strcmp_path(args[0], "cd") == 1)
        cd_cmd(args);
    else if (mini_strcmp_path(args[0], "echo") == 1)
        echo_cmd(args, env_var);
    else if (mini_strcmp_path(args[0], "unset") == 1)
    {
        if (args[1])
            unset_cmd(args[1], env);
    }
    else if (mini_strcmp_path(args[0], "env") == 1)
        print_nodes(env);
    else if (mini_strcmp_path(args[0], "export") == 1)
    {
        if (args[1])
            export_cmd(args[1], env, env_var);
    }
    else if (is_variable(args))
        add_list_variable(args, env_var);
    else
        return (1);

    return (0);
}
```

## 3. env.c Dosyasındaki Sorunlar

**Dosya:** `src/f_builtins/env.c`

### Mevcut Sorunlar:

1. **Eksik Koşul Yönetimi**: `create_env` fonksiyonunda `*env != NULL` durumunda, `new_node` değişkeni tanımlanıyor ancak kullanılmıyor.
2. **Bellek Sızıntısı Riski**: `env_lengths_2` fonksiyonunda bellek tahsis hataları `exit(1)` ile sonlandırılıyor, bu bellek temizliği yapmadan programın sonlanmasına neden olabilir.

### Çözüm Önerisi:

```c
void create_env(t_env **env)
{
    char **env_var;
    int i;

    i = 0;
    env_var = __environ;
    while (env_var[i] != NULL)
    {
        if (*env != NULL)
        {
            t_env *last = mini_lstlast(*env);
            t_env *new_node = NULL;
            add_variable_2(env_var[i], &new_node);
            last->next = new_node;
        }
        else
            add_variable_2(env_var[i], env);
        i++;
    }
}
```

## 4. export_cmd Fonksiyonundaki Sorunlar

**Dosya:** `src/f_builtins/export.c`

### Mevcut Sorunlar:

1. **Null Argüman Kontrolü Yok**: `arg` parametresi NULL olabilir.
2. **Mantık Hatası**: `compare_env` fonksiyonu her zaman 0 döner ama sadece bir uyuşma bulduğunda.
3. **Bellek Sızıntısı**: `free(tmp1)` ve `free(tmp2)` ifadeleri, NULL olabilen veya önceden serbest bırakılmış pointer'ları serbest bırakmaya çalışabilir.

### Çözüm Önerisi:

```c
int compare_env(t_env **env, t_env **env_var)
{
    t_env *tmp1;
    t_env *tmp2;

    if (!env || !*env || !env_var || !*env_var)
        return (0);
        
    tmp2 = *env_var;
    while (tmp2 != NULL)
    {
        tmp1 = *env;
        while (tmp1 != NULL)
        {
            if (mini_strcmp_path(tmp1->key, tmp2->key) == 1)
                return (0);
            tmp1 = tmp1->next;
        }
        tmp2 = tmp2->next;
    }
    return (1);
}

void export_cmd(char *arg, t_env **env, t_env **env_var)
{
    t_env *tmp;
    t_env *new_node;

    if (!arg || !env || !env_var || !*env_var)
        return;
        
    if (compare_env(env, env_var) == 0)
        return;
        
    tmp = *env_var;
    while (tmp != NULL)
    {
        if (mini_strcmp_path(tmp->key, arg) == 1)
        {
            t_env *last = mini_lstlast(*env);
            new_node = malloc(sizeof(t_env));
            if (!new_node)
                return;
            add_env_var(tmp->key, tmp->value, &new_node);
            last->next = new_node;
            return;
        }
        tmp = tmp->next;
    }
}
```

## 5. echo_cmd Fonksiyonundaki Sorunlar

**Dosya:** `src/f_builtins/echo.c`

### Mevcut Sorunlar:

1. **Null Argüman Kontrolü Yok**: `args[i]` erişiminde NULL kontrolü yok.
2. **Hata Yönetimi Eksik**: İşlevsel olmadan önce `i` indisinin array sınırları içinde olduğunu kontrol etmiyor.

### Çözüm Önerisi:

```c
void echo_cmd(char **args, t_env **env)
{
    int i;
    int n;

    if (!args || !args[1])
    {
        printf("\n");
        return;
    }

    i = 1;
    n = 0;
    
    if (mini_strcmp_path(args[i], "-n") == 1)
    {
        n = 1;
        i++;
    }
    
    if (args[i] && args[i][0] == '$')
    {
        env_value(args[i] + 1, *env, n);
        return;
    }
    
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    
    if (n == 0)
        printf("\n");
}
```

## Öneriler ve İyileştirmeler

1. **Bellek Yönetimi**: Tüm fonksiyonlarda bellek sızıntılarını önlemek için uygun bellek tahsisi ve serbest bırakma işlemleri yapılmalı.
2. **Hata Kontrolü**: Tüm fonksiyonlarda NULL veya geçersiz argüman kontrolleri yapılmalı.
3. **Kodlama Standardı**: 42 Kodlama Standardına uygun olarak kodun düzenlenmesi.
4. **Modüler Yapı**: Fonksiyonlar daha modüler ve tekrar kullanılabilir hale getirilmeli.
5. **Dokümantasyon**: Karmaşık algoritmaların açıklamalarının eklenmesi.
