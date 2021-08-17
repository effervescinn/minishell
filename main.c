#include "minishell.h"

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
    if (input)
        free(input);
    input = NULL;
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void history(t_info *info)
{
    char *prompt;
	char *newstr;
	char **tmp_arr;

    prompt = ft_strdup("dashBash$ ");
    while (1)
    {
        signal(SIGINT, &sig_int);
        input = NULL;
        input = readline(prompt);
        add_history(input);
        if (!input)
        {
            write(1, "\n", 1);
            exit(0);
        }
	    newstr = replace_vars(input, info);
	    tmp_arr = make_tokens(newstr);
	    info->tokens = delete_quotes(tmp_arr);
        define_types(info);
        program_define(info);
        // if (input)
            // free(input);
    }
}

int main(int ac, char **av, char **envp)
{
	t_info info;
	make_env(envp, &info.head);
    set_pointers(&info);
    info.exp = NULL;
    info.extra_exp = NULL;

    info.result = malloc(1);
    info.result[0] = '\0';
	info.i = 0;
    history(&info);
}

// gcc -L./libft -lft -ltermcap -lreadline -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include main.c