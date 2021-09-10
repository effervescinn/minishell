#include "minishell.h"

void add_oldpwd(t_info *info)
{
    t_list *tmp;

    if (!info->oldpwd_flag && !info->oldpwd)
    {
        tmp = info->head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = malloc(sizeof(t_list));
        tmp = tmp->next;
        tmp->content = ft_strdup("OLDPWD=");
        tmp->next = NULL;
        info->oldpwd_flag = 1;
        set_pointers(info);
    }
}

void new_pwd_frst(t_info *info)
{
    add_oldpwd(info);
    if (info->oldpwd)
    {
        if (info->str_oldpwd)
        {
            free(info->str_oldpwd);
            info->str_oldpwd = NULL;
            if (info->pwd)
                info->str_oldpwd = ft_strdup(info->pwd->content);
        }
        if (info->oldpwd)
        {
            free(info->oldpwd->content);
            if (info->str_pwd)
                info->oldpwd->content = ft_strjoin("OLD", info->str_pwd);
            else
                info->oldpwd->content = ft_strdup("OLDPWD=");
        }
    }
}

void new_pwd_scnd(t_info *info, char *buf)
{
    if (info->str_pwd)
        free(info->str_pwd);
    info->str_pwd = ft_strjoin("PWD=", buf);
    if (info->pwd)
    {
        free(info->pwd->content);
        info->pwd->content = ft_strdup(info->str_pwd);
    }
    free(buf);
}

void cd_problem(char **buf, t_info *info)
{
    int a;

    while (!(*buf))
    {
        a = chdir("..");
        if (!a)
        {
            new_pwd_frst(info);
            *buf = getcwd(NULL, 100);
            if (*buf && info->pwd)
                new_pwd_scnd(info, *buf);
        }
    }
}

void cd(t_info *info)
{
    char *buf;
    int a;

    if (!info->tokens[info->i].args[1])
        return;
    a = chdir(info->tokens[info->i].args[1]);
    if (!a)
    {
        new_pwd_frst(info);
        buf = getcwd(NULL, 100);
        if (buf && info->pwd)
            new_pwd_scnd(info, buf);
        else if (!buf)
            cd_problem(&buf, info);
    }
    else
        opening_error_scnd(info->tokens[info->i].args[1]);
}