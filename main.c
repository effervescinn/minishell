#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "funcs.c"
#include <term.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_str
{
    char *input;
}              t_str;

typedef void (*sighandler_t)(int);

t_str str;

int ft_putchar(int c)
{
    write(1, &c, 1);
    return 1;
}

void sig_int(int d)
{
    int right = 11;

    while (right--)
            tputs(cursor_right, 1, ft_putchar);
    tputs(tgetstr("dc", NULL), 1, ft_putchar);
    tputs(cursor_left, 1, ft_putchar);
    tputs(tgetstr("dc", NULL), 1, ft_putchar);
    write(1, "\n", 1);
    if (str.input)
        free(str.input);
    str.input = NULL;
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

char *history()
{
    char *prompt;

    prompt = ft_strdup("dashBash$ ");

    while (1)
    {
        signal(SIGINT, &sig_int);
        str.input = NULL;
        str.input = readline(prompt);
        add_history(str.input);
        if (!str.input)
        {
            write(1, "\n", 1);
            exit(0);
        }
        //тут должен быть парсер
        if (str.input)
            free(str.input);
    }
}

int main()
{
    history();
}

// gcc main.c -lreadline -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include