#include "lexer.h"

static void print_token_type(t_token_type type)
{
    if (type == WORD)
        printf("Type: WORD, ");
    else if (type == PIPE)
        printf("Type: PIPE, ");
    else if (type == REDIR_OUT)
        printf("Type: REDIR_OUT, ");
    else if (type == REDIR_IN)
        printf("Type: REDIR_IN, ");
    else if (type == REDIR_APPEND)
        printf("Type: REDIR_APPEND, ");
    else if (type == REDIR_HEREDOC)
        printf("Type: REDIR_HEREDOC, ");
}

void print_tokens(const t_token *head)
{
    while (head)
    {
        print_token_type(head->type);
        printf("Value: '%s'\n", head->value);
        head = head->next;
    }
}
