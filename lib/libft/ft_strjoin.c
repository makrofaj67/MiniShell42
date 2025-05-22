#include "../../inc/__minishell.h"

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
