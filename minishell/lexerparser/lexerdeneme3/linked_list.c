#include "lexer.h"

void add_token(t_token **head, t_token_type type, const char *value)
{
    t_token *new_token;
    t_token *tmp;

    new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
    {
        fprintf(stderr, "Malloc error\n");
        exit(1);
    }
    new_token->type = type;
    new_token->value = strdup(value);
    new_token->next = NULL;
    if (!*head)
        *head = new_token;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_token;
    }
}

void free_tokens(t_token *head)
{
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
}
