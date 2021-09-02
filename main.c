#include "minishell.h"

int ft_putchar(int c)
{
    write(1, &c, 1);
    return 1;
}

void sig_int(int d)
{
    int right = 50;
    int left = 40;

    while (right--)
        tputs(cursor_right, 1, ft_putchar);
    while (left--)
    {
        tputs(cursor_left, 1, ft_putchar);
        tputs(tgetstr("dc", NULL), 1, ft_putchar);
    }
    write(1, "\n", 1);
    if (g_global.input)
        free(g_global.input);
    g_global.input = NULL;
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    if (g_global.f)
    {
        int backspase = 10;
        while (backspase--)
        {
            tputs(cursor_left, 1, ft_putchar);
            tputs(tgetstr("dc", NULL), 1, ft_putchar);
        }
    }
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
        if (tokens[i].type[0] == 'p')
        {
            if (i == 0)
                return (1);
            if (tokens[i + 1].type)
                if (tokens[i + 1].type[0] == 'p')
                    return (1);
        }
        else if (tokens[i].type[0] == 'g' || tokens[i].type[0] == 'G' || tokens[i].type[0] == 'l' || tokens[i].type[0] == 'L')
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

char *close_pipe(char *line)
{
    int i;
    char buf[1024 + 1];
    int ret = 0;
    char *closed_str;
    char *no_enters;
    int j;
    char *tmp;

    closed_str = ft_substr(line, 0, ft_strlen(line));
    i = 0;
    while (line[i])
        i++;
    i--;
    while (line[i] == ' ')
        i--;
    if (line[i] == '|')
    {
        while((ret = read(0, buf, 1024)))
        {
            buf[ret] = '\0';
            j = ret - 2;
            while (buf[j] == ' ' && j > 0)
                j--;
            tmp = closed_str;
            closed_str = ft_strjoin(tmp, buf);
            free(tmp);
            if (buf[j] == '|')
                continue ;
            else
                break;         
        }
        no_enters = (char*)malloc(ft_strlen(closed_str) + 1);
        i = 0;
        while (*closed_str)
        {
            if (*closed_str != '\n')
            {
                no_enters[i] = *closed_str;
                i++;
            }
            closed_str++;
        }
        no_enters[i] = '\0';
        free(line);
        // free(closed_str);
        return (no_enters);
    }
    free(line);
    return closed_str;
}

void history(t_info *info)
{
    char *newstr;
    char **tmp_arr;
    char *closed_str;
    int tokens_err;

    g_global.prompt = ft_strdup("dashBash$ ");
    while (1)
    {
        info->i = 0;
        signal(SIGINT, &sig_int);
        signal(SIGQUIT, SIG_IGN);
        g_global.input = NULL;
        g_global.input = readline(g_global.prompt);
        g_global.input = close_pipe(g_global.input); //вот что добавила, сломался ctrl+D
        add_history(g_global.input);
        if (!g_global.input)
        {
            free_list(&info->extra_exp);
            free_list(&info->head);
            free(info->str_pwd);
            free(info->str_oldpwd);
            free(g_global.prompt);
            write(1, "\n", 1);
            exit(0);
        }
        if (count_quotes(g_global.input) % 2 == 0)
        {
            newstr = replace_vars(g_global.input, info);
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
                command_types(info);
                program_define(info);
                free_args(info);
                free_tokens(info);
            }
            else
                printf_tokens_err(tokens_err);
        }
        else
            write(1, "-dashBash: unclosed quote\n", 27);
        if (g_global.input)
        {
            free(g_global.input);
            g_global.input = NULL;
        }
    }
}

int main(int ac, char **av, char **envp)
{
    t_info info;
	info.result = 0;
    make_env(envp, &info.head);
    set_pointers(&info);
    make_paths(&info);
    info.oldpwd_flag = 0;
    g_global.f = 0;
    info.heredoc = NULL;

    // file = open("file", O_CREAT | O_WRONLY | O_TRUNC, 0777);
    info.str_oldpwd = NULL;
    info.str_pwd = NULL;
    copy_pwds(&info);

    info.exp = NULL;
    info.extra_exp = NULL;

    history(&info);
}
