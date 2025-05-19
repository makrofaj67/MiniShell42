
#include "../../inc/__minishell.h"
#include <stdlib.h> 


char *get_concated(char *command)
{
    if (!command)
        return (NULL);

    size_t command_len = ft_strlen(command);
    char *concated_str = (char *)malloc(sizeof(char) * (command_len + 1));
    if (!concated_str)
        return (NULL);

    int i = 0; 
    int j = 0; 
    int quote_state = 0; 

    while (command[i])
    {
        if (quote_state == 0) 
        {
            if (command[i] == '\'')
            {
                quote_state = 1; 
                i++;             
            }
            else if (command[i] == '"')
            {
                quote_state = 2; 
                i++;             
            }
            else
            {
                concated_str[j++] = command[i++]; 
            }
        }
        else if (quote_state == 1) 
        {
            if (command[i] == '\'')
            {
                quote_state = 0; 
                i++;             
            }
            else
            {
                concated_str[j++] = command[i++]; 
            }
        }
        else 
        {
            if (command[i] == '"')
            {
                quote_state = 0; 
                i++;             
            }
            else
            {
                concated_str[j++] = command[i++]; 
            }
        }
    }
    concated_str[j] = '\0';
    printf("%s", concated_str);
    return (concated_str);
}
