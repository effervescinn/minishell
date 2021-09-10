#include "minishell.h"

void check_pipe()
{
    int i;

    if (g_global.input == NULL)
        return;
    i = 0;
    while (g_global.input[i])
        i++;
    if (i > 0)
        i--;
    while (g_global.input[i] == ' ' && i > 0)
        i--;
    if (g_global.input[i] == '|')
    {
        if (i > 0)
            i--;
        while (g_global.input[i] == ' ' && i > 0)
            i--;
        if (g_global.input[i] == '|')
            return ;
    }
    g_global.input = close_pipe(g_global.input);
    return ;
}

void sig_int(int sig)
{
	if (g_global.f)
        signal(SIGINT, SIG_IGN);
	if (!g_global.f)
	{
        int left = 40;
        int right = 50;

        while (right--)
            tputs(cursor_right, 1, ft_putchar);
        while (left--)
        {
            tputs(cursor_left, 1, ft_putchar);
            tputs(tgetstr("dc", NULL), 1, ft_putchar);
        }
		write(1, "\n", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}