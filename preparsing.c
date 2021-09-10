#include "minishell.h"

int unexpected_return(t_token *tokens, int i)
{
    if (tokens[i + 1].str)
    {
        if (tokens[i + 1].type == 'p')
            return (1);
        else if (tokens[i + 1].type == 'L')
            return (2);
        else if (tokens[i + 1].type == 'l')
            return (3);
        else if (tokens[i + 1].type == 'g')
            return (4);
        else if (tokens[i + 1].type == 'G')
            return (5);
    }
    else
        return (6);
    return (0);
}

char *make_no_enters_string(char **closed_str, char **line)
{
    char *no_enters;
    int i;
    int j;

    no_enters = (char *)malloc(ft_strlen(*closed_str) + 1);
    i = 0;
    j = 0;
    while ((*closed_str)[j])
    {
        if ((*closed_str)[j] != '\n')
        {
            no_enters[i] = (*closed_str)[j];
            i++;
        }
        j++;
    }
    no_enters[i] = '\0';
    free(*line);
    free(*closed_str);
    return (no_enters);
}

int make_closed_string(char **closed_str)
{
    int ret;
    char buf[1024 + 1];
    int j;
    char *tmp;

    ret = 1;
    while (ret)
    {
        write(1, "> ", 2);
        ret = read(0, buf, 1024);
        buf[ret] = '\0';
        j = ret - 2;
        if (!ret)
            return (0);
        while (buf[j] == ' ' && j > 0)
            j--;
        tmp = *closed_str;
        *closed_str = ft_strjoin(tmp, buf);
        free(tmp);
        if (buf[j] == '|')
            continue ;
        else
            break;         
    }
    return (1);
}

char *close_pipe(char *line)
{
    int i;
    char *closed_str;

    if (!line)
        return (NULL);
    closed_str = ft_substr(line, 0, ft_strlen(line));
    i = 0;
    while (line[i])
        i++;
    if (i == 0)
        return (line);
    i--;
    while (i > 0 && line[i] == ' ')
        i--;
    if (line[i] == '|')
    {
        if (!make_closed_string(&closed_str))
            return (NULL);
        return (make_no_enters_string(&closed_str, &line));
    }
    free(line);
    return (closed_str);
}
