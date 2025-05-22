#include "../../inc/__minishell.h"

int count_digits(long n)
{
    int count = 0;
    
    if (n == 0)
        return 1;
    if (n < 0)
    {
        count++;
        n = -n;
    }
    while (n > 0)
    {
        n /= 10;
        count++;
    }
    return (count);
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
