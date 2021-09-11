#include "../minishell.h"

void printf_tokens_err(int error)
{
    if (error == 1)
        printf("-dashBash: syntax error near unexpected token `|'\n");
    else if (error == 2)
        printf("-dashBash: syntax error near unexpected token `<<'\n");
    else if (error == 3)
        printf("-dashBash: syntax error near unexpected token `<'\n");
    else if (error == 4)
        printf("-dashBash: syntax error near unexpected token `>'\n");
    else if (error == 5)
        printf("-dashBash: syntax error near unexpected token `>>'\n");
    else if (error == 6)
        printf("-dashBash: syntax error near unexpected token `newline'\n");
    g_global.ex_status = 258;
}

int define_error(t_token *tokens, int *i)
{
    if (tokens[*i + 1].type == 'p')
        return (1);
    else if (tokens[*i + 1].type == 'L')
        return (2);
    else if (tokens[*i + 1].type == 'l')
        return (3);
    else if (tokens[*i + 1].type == 'g')
        return (4);
    else if (tokens[*i + 1].type == 'G')
        return (5);
    return (0);
}

int unexpected_pipe(t_token **tokens, int *i)
{
    if ((*tokens)[*i].type == 'p')
    {
        if (*i == 0)
            return (1);
        if ((*tokens)[*i + 1].str)
            if ((*tokens)[*i + 1].type == 'p')
                return (1);
    }
    return (0);
}

int unexpected_tokens(t_token *tokens)
{
    int i;
    int ret;

    i = 0;
    while (tokens[i].str)
    {
        if (unexpected_pipe(&tokens, &i))
            return (1);
        else if (tokens[i].type == 'g' || tokens[i].type == 'G'
        || tokens[i].type == 'l' || tokens[i].type == 'L')
        {
            if (tokens[i + 1].str)
            {
                ret = define_error(tokens, &i);
                if (ret == 1 || ret == 2 | ret == 3 || ret == 4 || ret == 5)
                    return (ret);
            }
            else
                return (6);
        }
        i++;
    }
    return (0);
}
