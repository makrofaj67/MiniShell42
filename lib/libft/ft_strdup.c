#include "../../inc/__minishell.h"

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
