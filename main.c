#include "minishell.h"

t_global g_global;

void make_env(char **envp, t_list **head)
{
	t_list *new;
	int i;

	i = 1;
	*head = ft_lstnew(ft_strdup(envp[0]));
	while (envp[i])
	{
		new = (t_list *)malloc(sizeof(t_list));
		if (ft_strncmp("_=./", envp[i], 4) && ft_strncmp("OLDPWD=", envp[i], 7))
		{
			if (!ft_strncmp(envp[i], "SHLVL=", 6))
				new->content = change_shlvl(envp[i]);
			else
				new->content = ft_strdup(envp[i]);
			new->next = NULL;
			ft_lstadd_back(head, new);
		}
		else
			free(new);
		i++;
	}
}

void execution(t_info *info)
{
    int tokens_err;

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

void parse(t_info *info)
{
    char *newstr;
    char **tmp_arr;

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
        execution(info);
    }
    else if (count_quotes(g_global.input) % 2 != 0)
        write(1, "-dashBash: unclosed quote\n", 27);
}

void history(t_info *info)
{
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
        parse(info);
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

    (void)ac;
    (void)av;
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
