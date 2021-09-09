#include "minishell.h"

int check_flags(char **tmp_token, int *i, int *flag)
{
    if ((*tmp_token)[*i] == '\'' && *flag == 0)
    {
        *flag = 1;
        (*i)++;
        return (1);
    }
    else if ((*tmp_token)[*i] == '\'' && *flag == 1)
    {
        *flag = 0;
        (*i)++;
        return (1);
    }
    else if ((*tmp_token)[*i] == '\"' && *flag == 0)
    {
        *flag = 2;
        (*i)++;
        return (1);
    }
    else if ((*tmp_token)[*i] == '\"' && *flag == 2)
    {
        *flag = 0;
        (*i)++;
        return (1);
    }
    return (0);
}

void handle_token(char *tmp_token, char **str)
{
    int i;
    int j;
    int quotes;
    int flag;

    i = 0;
    j = 0;
    flag = 0;
    while (tmp_token[i])
    {
        if (check_flags(&tmp_token, &i, &flag))
            continue;
        if (flag == 1)
        {
            while (tmp_token[i] && tmp_token[i] != '\'')
            {
                (*str)[j] = tmp_token[i];
                i++;
                j++;
            }
            continue;
        }
        else if (flag == 2)
        {
            while (tmp_token[i] && tmp_token[i] != '\"')
            {
                (*str)[j] = tmp_token[i];
                i++;
                j++;
            }
            continue;
        }
        else
        {
            while (tmp_token[i] && tmp_token[i] != '\"' && tmp_token[i] != '\'')
            {
                (*str)[j] = tmp_token[i];
                i++;
                j++;
            }
            continue;
        }
        if (tmp_token[i])
            i++;
    }
    (*str)[j] = '\0';
}

void set_types(char ***tmp_arr, t_token **tokens_arr, int *i)
{
    if ((*tmp_arr)[*i][0] == '|' && (*tmp_arr)[*i][1] == '\0')
        (*tokens_arr)[*i].type = 'p';
    else if ((*tmp_arr)[*i][0] == '>' && (*tmp_arr)[*i][1] == '\0')
        (*tokens_arr)[*i].type = 'g';
    else if ((*tmp_arr)[*i][0] == '<' && (*tmp_arr)[*i][1] == '\0')
        (*tokens_arr)[*i].type = 'l';
    else if ((*tmp_arr)[*i][0] == '>' && (*tmp_arr)[*i][1] == '>' && (*tmp_arr)[*i][2] == '\0')
        (*tokens_arr)[*i].type = 'G';
    else if ((*tmp_arr)[*i][0] == '<' && (*tmp_arr)[*i][1] == '<' && (*tmp_arr)[*i][2] == '\0')
        (*tokens_arr)[*i].type = 'L';
    else
        (*tokens_arr)[*i].type = 'w';
}

t_token *delete_quotes(char **tmp_arr)
{
    int i;
    t_token *tokens_arr;

    i = 0;
    while (tmp_arr[i])
        i++;
    tokens_arr = (t_token *)malloc(sizeof(t_token) * (i + 1));
    tokens_arr[i].str = NULL;
    i = -1;
    while (tmp_arr[++i])
    {
        tokens_arr[i].str = (char *)malloc(ft_strlen(tmp_arr[i]) + 1);
        handle_token(tmp_arr[i], &tokens_arr[i].str);
        set_types(&tmp_arr, &tokens_arr, &i);
    }
    return (tokens_arr);
}
