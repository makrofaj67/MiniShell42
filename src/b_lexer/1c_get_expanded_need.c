#include "../../inc/__minishell.h"

int is_need_for_expanding(char *raw_command, int i)
{
    int quote_state;
    if (i > 0 && raw_command[i - 1] == '\\')
    {
        quote_state = get_quote_state_at_position(raw_command, i - 1);
        if (quote_state == 0 || quote_state == 2)
            return (0);
    }
    if (raw_command[i + 1] == '\0')
        return (0);
    if (raw_command[i + 1] == '$')
        return (0);
    if (raw_command[i + 1] == ' ' || raw_command[i + 1] == '\t' || 
        raw_command[i + 1] == '/' || raw_command[i + 1] == '=' ||
        raw_command[i + 1] == ':' || raw_command[i + 1] == ';' ||
        raw_command[i + 1] == ',' || raw_command[i + 1] == '.')
        return (0);
    quote_state = get_quote_state_at_position(raw_command, i);
    if (quote_state == 1)
        return (0);
    if (raw_command[i + 1] == '?')
        return (1);
    if ((raw_command[i + 1] >= 'a' && raw_command[i + 1] <= 'z') ||
        (raw_command[i + 1] >= 'A' && raw_command[i + 1] <= 'Z') ||
        raw_command[i + 1] == '_')
        return (1);
    return (0);
}
