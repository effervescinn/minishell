#include "minishell.h"

void pipe_redir(char **str, char ***arr, char **start, int *i)
{
    char **tmp;
    int j;

    (*i)++;
    tmp = *arr;
    *arr = (char **)malloc(sizeof(char *) * (*i + 1));
    (*arr)[*i] = NULL;
    j = -1;
    while (tmp[++j])
        (*arr)[j] = ft_substr(tmp[j], 0, ft_strlen(tmp[j]));
    if (*(*str + 1) == '>' || *(*str + 1) == '<')
    {
        (*arr)[j] = ft_substr(*start, 0, 2);
        *str += 2;
    }
    else
    {
        (*arr)[j] = ft_substr(*start, 0, 1);
        (*str)++;
    }
    j = -1;
    while (++j < *i)
        free(tmp[j]);
    free(tmp);
}

void meet_quotes(char **str)
{
    if (**str == '\'')
    {
        (*str)++;
        *str = ft_strchr(*str, '\'');
        while (**str && **str != ' ' && **str != '|'
        && **str != '<' && **str != '>')
            (*str)++;
    }
    else if (**str == '\"')
    {
        (*str)++;
        *str = ft_strchr(*str, '\"');
        while (**str && **str != ' ' && **str != '|'
        && **str != '<' && **str != '>')
            (*str)++;
    }
}

void quotes_after(char **str)
{
    while (**str && **str != ' ')
    {
        while (**str && **str != '\"' && **str != '\'' && **str != ' '
        && **str != '|' && **str != '>' && **str != '<')
            (*str)++;
        if (**str == '\'')
        {
            (*str)++;
            *str = ft_strchr(*str, '\'');
            (*str)++;
        }
        else if (**str == '\"')
        {
            (*str)++;
            *str = ft_strchr(*str, '\"');
            (*str)++;
        }
        else if (**str == '|' || **str == '>' || **str == '<')
            break;
    }
}

void finish_tokens(char **str, char ***arr, char **start, int *i)
{
    char **tmp;
    int j;

    tmp = *arr;
    *arr = (char **)malloc(sizeof(char *) * (*i + 1));
    (*arr)[*i] = NULL;
    j = 0;
    while (tmp[j])
    {
        (*arr)[j] = ft_substr(tmp[j], 0, ft_strlen(tmp[j]));
        j++;
    }
    (*arr)[j] = ft_substr(*start, 0, *str - *start);
    j = 0;
    while (j < *i)
    {
        free(tmp[j]);
        j++;
    }
    free(tmp);
}
