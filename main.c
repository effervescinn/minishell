#include "minishell.h"

t_global g_global;

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
        check_pipe();
        add_history(g_global.input);
        if (!g_global.input)
            exit_ctrl_d(info);
        if (count_quotes(g_global.input) % 2 == 0 && ft_strlen(g_global.input))
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
                set_start(info);
                program_define(info);
                free_args(info);
                free_tokens(info);
            }
            else
                printf_tokens_err(tokens_err);
        }
        else if (count_quotes(g_global.input) % 2 != 0)
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

    make_env(envp, &info.head);
    set_pointers(&info);
    make_paths(&info);
    info.oldpwd_flag = 0;
    g_global.hd_ex = 0;
    g_global.f = 0;
    info.heredoc = NULL;
    info.str_oldpwd = NULL;
    info.str_pwd = NULL;
    copy_pwds(&info);
    info.exp = NULL;
    info.extra_exp = NULL;
    history(&info);
}
