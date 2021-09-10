#include "minishell.h"

void env(t_info *info)
{
    t_list *tmp;

    tmp = info->head;
    while (tmp)
    {
        info->result = no_leaks_join(info->result, tmp->content);
        info->result = no_leaks_join(info->result, "\n");
        tmp = tmp->next;
    }
}

void pwd(t_info *info)
{
    char *buf = NULL;

    buf = getcwd(NULL, 100);
    if (buf)
    {
        info->result = no_leaks_join(info->result, buf);
        info->result = no_leaks_join(info->result, "\n");
    }
    while (!buf)
    {
        chdir("..");
        buf = getcwd(NULL, 100);
        info->result = no_leaks_join(info->result, buf);
        info->result = no_leaks_join(info->result, "\n");
    }
    free(buf);
}