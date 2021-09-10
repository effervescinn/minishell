#include "minishell.h"

int unexpected_tokens(t_token *tokens)
{
    int i;

    i = 0;
    while (tokens[i].str)
    {
        if (tokens[i].type == 'p')
        {
            if (i == 0)
                return (1);
            if (tokens[i + 1].str)
                if (tokens[i + 1].type == 'p')
                    return (1);
        }
        else if (tokens[i].type == 'g' || tokens[i].type == 'G' || tokens[i].type == 'l' || tokens[i].type == 'L')
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
        }
        i++;
    }
    return (0);
}

// char make_no_enters_string()
// {
//     no_enters = (char*)malloc(ft_strlen(closed_str) + 1);
//     i = 0;
//     j = 0;
//     while (closed_str[j])
//         {
//             if (closed_str[j] != '\n')
//             {
//                 no_enters[i] = closed_str[j];
//                 i++;
//             }
//             j++;
//         }
//         no_enters[i] = '\0';
//         free(line);
//         free(closed_str);
//         return (no_enters);
// }

char *close_pipe(char *line)
{
    int i;
    char buf[1024 + 1];
    int ret = 0;
    char *closed_str;
    char *no_enters;
    int j;
    char *tmp;

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
        j = 0;
        while (closed_str[j])
        {
            if (closed_str[j] != '\n')
            {
                no_enters[i] = closed_str[j];
                i++;
            }
            j++;
        }
        no_enters[i] = '\0';
        free(line);
        free(closed_str);
        return (no_enters);
    }
    free(line);
    return (closed_str);
}

void check_pipe()
{
    int i;

    if (g_global.input == NULL)
        return;
    i = 0;
    while (g_global.input[i])
        i++;
    if (i > 0)
        i--;
    while (g_global.input[i] == ' ' && i > 0)
        i--;
    if (g_global.input[i] == '|')
    {
        if (i > 0)
            i--;
        while (g_global.input[i] == ' ' && i > 0)
            i--;
        if (g_global.input[i] == '|')
            return ;
    }
    g_global.input = close_pipe(g_global.input);
    return ;
}