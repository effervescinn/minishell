#include "minishell.h"

char *heredoc_str(char *stop, char *buf, int *len)
{

    write (0, "> ", 2);
    *len = read(0, buf, 100);
    buf[*len] = '\0';
    if (!ft_strncmp(buf, stop, ft_strlen(stop)))
        return (NULL);
    return (buf);
}

void heredoc(t_info *info, char *stop)
{
    char *str;
    char *buf;
    int len;

    buf = malloc(sizeof(char) * 101);
    str = heredoc_str(stop, buf, &len);
    while (str)
        str = heredoc_str(stop, buf, &len);
    free(buf);
}

void search_heredoc(t_info *info)
{
    int i;
    i = info->i2;
    while (info->tokens[i].str && info->tokens[i].type[0] != 'p')
    {
        if (info->tokens[i].type[0] == 'L')
            heredoc(info, info->tokens[i].args[0]);
        i++;
    }
}