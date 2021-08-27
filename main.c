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

void free_tokens(t_info *info)
{
    int i;

    i = 0;
    while (info->tokens[i].str)
    {
        free(info->tokens[i].str);
        i++;
    }
    free(info->tokens);
}

void free_args(t_info *info)
{
    int i;
    int j;

    i = 0;
    while (info->tokens[i].str)
    {
        j = 0;
        while (info->tokens[i].args[j])
        {
            free(info->tokens[i].args[j]);
            j++;
        }
        free(info->tokens[i].args);
        i++;
    }
}

int unexpected_tokens(t_token *tokens)
{
    int i;

    i = 0;
    while (tokens[i].str)
    {
        if (tokens[i].type[0] == 'g' || tokens[i].type[0] == 'G' || tokens[i].type[0] == 'l' || tokens[i].type[0] == 'L')
        {
            if (tokens[i + 1].type)
            {
                if (tokens[i + 1].type[0] == 'p')
                    return (1);
                else if (tokens[i + 1].type[0] == 'L')
                    return (2);
                else if (tokens[i + 1].type[0] == 'l')
                    return (3);
                else if (tokens[i + 1].type[0] == 'g')
                    return (4);
                else if (tokens[i + 1].type[0] == 'G')
                    return (5);
            }
            else
                return (6);
        }
        i++;
    }
    return (0);
}

void printf_tokens_err(int error)
{
    if (error == 1)
        printf("dashBash: syntax error near unexpected token `|'\n");
    else if (error == 2)
        printf("dashBash: syntax error near unexpected token `<<'\n");
    else if (error == 3)
        printf("dashBash: syntax error near unexpected token `<'\n");
    else if (error == 4)
        printf("dashBash: syntax error near unexpected token `>'\n");
    else if (error == 5)
        printf("dashBash: syntax error near unexpected token `>>'\n");
    else if (error == 6)
        printf("dashBash: syntax error near unexpected token `newline'\n");
}

void history(t_info *info)
{
    char *prompt;
    char *newstr;
    char **tmp_arr;
    int tokens_err;

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
        if (count_quotes(input) % 2 == 0)
        {
            newstr = replace_vars(input, info);
            tmp_arr = make_tokens(newstr);
            info->tokens = delete_quotes(tmp_arr);
            free(newstr);
            int i = 0;
            while (tmp_arr[i])
            {
                free(tmp_arr[i]);
                i++;
            }
            free(tmp_arr);
            tokens_err = unexpected_tokens(info->tokens);
            if (tokens_err == 0)
            {
                set_args(info);
                define_types(info);
                set_pipes(info);
                program_define(info);
                free_args(info);
                free_tokens(info);
            }
            else
                printf_tokens_err(tokens_err);
        }
        else
            write(1, "-dashBash: unclosed quote\n", 27);
        // if (input)
        // free(input);
    }
}

int main(int ac, char **av, char **envp)
{
    t_info info;
    make_env(envp, &info.head);
    set_pointers(&info);
    make_paths(&info);

    file = open("file", O_CREAT | O_WRONLY | O_TRUNC, 0777);
    info.str_oldpwd = NULL;
    info.str_pwd = NULL;
    copy_pwds(&info);

    info.exp = NULL;
    info.extra_exp = NULL;

    info.fd_out_copy = dup(1);
    info.fd_in_copy = dup(0);

    info.result = ft_strdup("\0");
    info.i = 0;
    history(&info);
}