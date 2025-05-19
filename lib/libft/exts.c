#include "../../inc/__minishell.h"

size_t ft_strlen(const char *s)
{
    size_t i = 0;
    while (s && s[i])
        i++;
    return (i);
}

char *ft_strcpy(char *dst, const char *src)
{
    size_t i = 0;
    if (!dst || !src)
        return (NULL);
    while (src[i])
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (dst);
}

char *ft_strdup(const char *s1)
{
    char *dup;
    size_t len;

    if (!s1)
        return (NULL);
    len = ft_strlen(s1);
    dup = (char *)malloc(sizeof(char) * (len + 1));
    if (!dup)
        return (NULL);
    ft_strcpy(dup, s1);
    return (dup);
}

char *ft_substr(char const *s, unsigned int start, size_t len)
{
    char *sub;
    size_t s_len;
    size_t i;

    if (!s)
        return (NULL);
    s_len = ft_strlen(s);
    if (start >= s_len)
        return (ft_strdup(""));
    if (len > s_len - start)
        len = s_len - start;
    sub = (char *)malloc(sizeof(char) * (len + 1));
    if (!sub)
        return (NULL);
    i = 0;
    while (i < len && s[start + i])
    {
        sub[i] = s[start + i];
        i++;
    }
    sub[i] = '\0';
    return (sub);
}

char *ft_strjoin(char const *s1, char const *s2)
{
    char *join;
    size_t len1;
    size_t len2;

    if (!s1 && !s2)
        return (NULL);
    if (!s1)
        return (ft_strdup(s2));
    if (!s2)
        return (ft_strdup(s1));
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    join = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
    if (!join)
        return (NULL);
    ft_strcpy(join, s1);
    ft_strcpy(join + len1, s2);
    return (join);
}

int ft_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    
    if (!s1 || !s2)
    {
        if (s1 == s2) return 0;
        return (s1 ? 1 : -1); // Arbitrary non-zero if one is NULL
    }
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char *ft_itoa(int n)
{
    char *str;
    long num;
    int len;
    int is_negative;

    num = n;
    is_negative = 0;
    if (num < 0)
    {
        is_negative = 1;
        num = -num;
    }
    len = count_digits(n);
    str = (char *)malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);
    str[len] = '\0';
    if (num == 0)
        str[0] = '0';
    while (num > 0)
    {
        len--;
        str[len] = (num % 10) + '0';
        num /= 10;
    }
    if (is_negative)
        str[0] = '-';
    return (str);
}

int count_digits(long n)
{
    int count = 0;
    
    if (n == 0)
        return 1;  // 0 bir basamaktır   
    if (n < 0)
    {
        count++;  // eksi işareti için
        n = -n;   // sayıyı pozitife çeviriyoruz
    }
    while (n > 0)
    {
        n /= 10;  // basamakları saymak için 10'a bölüyoruz
        count++;
    }
    return (count);
}

