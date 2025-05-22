#include "../../inc/__minishell.h"

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
