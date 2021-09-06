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

void files_to_unlink(t_info *info, char *filename)
{
    t_list *tmp;

    if (!info->heredoc)
    {
        info->heredoc = ft_lstnew(ft_strdup(filename));
        return;
    }
    tmp = info->heredoc;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = malloc(sizeof(t_list));
    tmp = tmp->next;
    tmp->content = ft_strdup(filename);
    tmp->next = NULL;
}

void unlink_files(t_info *info)
{
    t_list *tmp;

    tmp = info->heredoc;
    while (tmp)
    {
        unlink(tmp->content);
        tmp = tmp->next;
    }
    free_list(&info->heredoc);
    info->heredoc = NULL;
}

void search_heredoc(t_info *info)
{
    int i;
    int fd;
    char *filename;

    i = 0;
    while (info->tokens[i].str)
    {
        if (info->tokens[i].type == 'L')
        {
            filename = ft_strjoin(info->tokens[i].args[0], ".tmp");
            fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0777);
            char *buf;
            char *str;
            int len;
            buf = malloc(sizeof(char) * 100);
            str = heredoc_str(info->tokens[i].args[0], buf, &len);
            while (str)
            {
                write(fd, buf, len);
                str = heredoc_str(info->tokens[i].args[0], buf, &len);
            }
            free(buf);
            close(fd);
            free(info->tokens[i].args[0]);
            info->tokens[i].args[0] = ft_strdup(filename);
            info->tokens[i].type = 'l';
            files_to_unlink(info, filename);
            free(filename);
        }
        i++;
    }
}

void redirects_solo(t_info *info)
{
    int i;
    int fd;
    int a;

    i = info->i;
    while (info->tokens[i].str && info->tokens[i].type != 'p')
    {
        if (info->tokens[i].type == 'g')
        {
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_TRUNC, 0777);
            a = write(fd, "\0", 1);
            if (a == -1)
                opening_error(info->tokens[i].args[0]);
            close(fd);
        }
        if (info->tokens[i].type == 'G')
        {
            fd = open(info->tokens[i].args[0], O_CREAT | O_WRONLY | O_APPEND, 0777);
            a = write(fd, "\0", 1);
            if (a == -1)
                opening_error(info->tokens[i].args[0]);
            close(fd);
        }
        if (info->tokens[i].type == 'l')
        {
            fd = open(info->tokens[i].args[0], O_RDONLY);
            if (fd == -1)
                opening_error(info->tokens[i].args[0]);
            close(fd);
        }
        i++;
    }
}

void join_result(t_info *info, char *str)
{
    if (info->result)
        no_leaks_join(info->result, str);
    else
        info->result = ft_strdup(str);
        
}