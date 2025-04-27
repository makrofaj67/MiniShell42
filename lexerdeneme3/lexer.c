#include "lexer.h"

static int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

static int is_pipe(char c)
{
    return (c == '|');
}

static int is_redir(const char *s)
{
    if (s[0] == '>' && s[1] == '>')
        return 1;
    if (s[0] == '<' && s[1] == '<')
        return 2;
    if (s[0] == '>')
        return 3;
    if (s[0] == '<')
        return 4;
    return 0;
}

static int handle_pipe(const char *input, t_token **tokens, size_t *i)
{
    if (is_pipe(input[*i]))
    {
        add_token(tokens, PIPE, "|");
        (*i)++;
        return 1;
    }
    return 0;
}

static int handle_redir(const char *input, t_token **tokens, size_t *i)
{
    int r = is_redir(&input[*i]);
    if (r == 1)
    {
        add_token(tokens, REDIR_APPEND, ">>");
        (*i) += 2;
    }
    else if (r == 2)
    {
        add_token(tokens, REDIR_HEREDOC, "<<");
        (*i) += 2;
    }
    else if (r == 3)
    {
        add_token(tokens, REDIR_OUT, ">");
        (*i)++;
    }
    else if (r == 4)
    {
        add_token(tokens, REDIR_IN, "<");
        (*i)++;
    }
    else
        return 0;
    return 1;
}

static void handle_word(const char *input, t_token **tokens, size_t *i)
{
    size_t start = *i;
    char quote = 0;
    if (input[*i] == '\'' || input[*i] == '"') {
        quote = input[*i];
        (*i)++;
        start = *i;
        while (input[*i] && input[*i] != quote)
            (*i)++;
        if (!input[*i]) {
            // Unclosed quote: take the rest as a token (bash-like)
            char *word = strndup(&input[start - 1], *i - start + 1); // include opening quote
            if (!word) {
                fprintf(stderr, "Malloc error\n");
                free_tokens(*tokens);
                exit(1);
            }
            add_token(tokens, WORD, word);
            free(word);
            return;
        }
        char *word = strndup(&input[start], *i - start);
        if (!word) {
            fprintf(stderr, "Malloc error\n");
            free_tokens(*tokens);
            exit(1);
        }
        add_token(tokens, WORD, word);
        free(word);
        (*i)++; // skip closing quote
    } else {
        while (input[*i] && !is_space(input[*i]) && !is_pipe(input[*i]) && !is_redir(&input[*i]) && input[*i] != '\'' && input[*i] != '"')
            (*i)++;
        char *word = strndup(&input[start], *i - start);
        if (!word) {
            fprintf(stderr, "Malloc error\n");
            free_tokens(*tokens);
            exit(1);
        }
        add_token(tokens, WORD, word);
        free(word);
    }
}

int has_unclosed_quote(const char *input)
{
    char quote = 0;
    size_t i = 0;
    while (input[i])
    {
        if ((input[i] == '"' || input[i] == '\'') && !quote)
            quote = input[i];
        else if (input[i] == quote)
            quote = 0;
        i++;
    }
    return (quote != 0);
}

t_token *lexer(const char *input)
{
    t_token *tokens = NULL;
    size_t i = 0;
    if (has_unclosed_quote(input))
        return NULL;
    while (input[i])
    {
        while (is_space(input[i]))
            i++;
        if (!input[i])
            break;
        if (handle_pipe(input, &tokens, &i))
            continue;
        if (handle_redir(input, &tokens, &i))
            continue;
        handle_word(input, &tokens, &i);
    }
    return tokens;
}
