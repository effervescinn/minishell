#include "minishell.h"

void opening_error(char *filename)
{
    write(2, "-dashBash: ", 11);
    write(2, filename, ft_strlen(filename));
    if (errno == 2)
        write(2, ": No such file or directory\n", 28);
    else if (errno == 13 || errno == 9)
        write(2, ": Permission denied\n", 21);
    else if (errno == 20)
        write(2, ": Not a directory\n", 18);
    else if (errno == 2)
        write(2, ": No such file or directory\n", 28);
    exit(1);
}

void opening_error_scnd(char *filename)
{
    write(2, "-dashBash: ", 11);
    write(2, filename, ft_strlen(filename));
    if (errno == 2)
        write(2, ": No such file or directory\n", 28);
    else if (errno == 13 || errno == 9)
        write(2, ": Permission denied\n", 21);
    else if (errno == 20)
        write(2, ": Not a directory\n", 18);
    else if (errno == 2)
        write(2, ": No such file or directory\n", 28);
    g_global.ex_status = 1;
}

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