#include "lexer.h"

int main(int argc, char **argv)
{
    t_token *tokens;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s \"command line\"\n", argv[0]);
        return (1);
    }
    if (has_unclosed_quote(argv[1]))
    {
        fprintf(stderr, "Syntax error: unclosed quote\n");
        return (2);
    }
    tokens = lexer(argv[1]);
    if (!tokens)
    {
        fprintf(stderr, "%s: command not found\n", argv[1]);
        return (2);
    }
    printf("Tokens:\n");
    print_tokens(tokens);
    free_tokens(tokens);
    return (0);
}
