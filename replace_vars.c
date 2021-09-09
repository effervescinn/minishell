#include "minishell.h"

char *vars(char **str, t_list *head)
{
    char *var;
    char *newvar;
    int j;
    int len;

    newvar = NULL;
    while (head)
    {
        j = 0;
        while ((*str)[j] && head->content[j] && head->content[j] == (*str)[j])
            j++;
        if (head->content[j] == '=' && ((*str)[j] == ' ' || (*str)[j] == '\0'
        || (*str)[j] == '\'' || (*str)[j] == '\"' || (*str)[j] == '$'))
        {
            j++;
            newvar = ft_substr(&(head->content[j]), 0,
            ft_strlen(&(head->content[j])));
            return (newvar);
        }
        head = head->next;
    }
    if (newvar == NULL)
        newvar = ft_strdup("\0");
    return (newvar);
}

void glue_var(char **str, char **newstr, char **start, t_info *info)
{
    char *tmp;
    char *glue;
    char *var;

    var = vars(str, info->head);
    glue = ft_substr(*start, 0, *str - *start - 1);
    tmp = *newstr;
    *newstr = ft_strjoin(tmp, glue);
    free(tmp);
    free(glue);
    tmp = *newstr;
    *newstr = ft_strjoin(tmp, var);
    free(tmp);
    free(var);
    while (**str != ' ' && **str != '\0' &&
    **str != '\'' && **str != '\"' && **str != '$')
        (*str)++;
}

void question(char **str, char **newstr, char **start)
{
    char *glue;
    char *tmp2;
    char *tmp;

    tmp = *newstr;
    glue = ft_substr(*start, 0, *str - *start - 1);
    *newstr = ft_strjoin(tmp, glue);
    free(tmp);
    free(glue);
    tmp = *newstr;
    tmp2 = ft_itoa(g_global.ex_status);
    *newstr = ft_strjoin(tmp, tmp2);
    free(tmp2);
    free(tmp);
    (*str)++;
}

void s_quote(char **str, char **newstr, char **start)
{
    char *tmp;
    char *glue;

    tmp = *newstr;
    glue = ft_substr(*start, 0, *str - *start);
    *newstr = ft_strjoin(tmp, glue);
    free(tmp);
    free(glue);
    *start = *str;
    while (**str != '\0' && **str != '\'' && **str != '\"' && **str != ' ')
        (*str)++;
    tmp = *newstr;
    glue = ft_substr(*start, 0, *str - *start);
    *newstr = ft_strjoin(tmp, glue);
    free(tmp);
    free(glue);
}

void dollar(char **str, char **newstr, char **start, t_info *info)
{
    char *tmp;
    char *glue;

    (*str)++;
    if (**str == ' ' || **str == '\0')
    {
        tmp = *newstr;
        glue = ft_substr(*start, 0, *str - *start);
        *newstr = ft_strjoin(tmp, glue);
        free(tmp);
        free(glue);
        return;
    }
    if ((info->d_quote == 0 && info->s_quote == 1))
        s_quote(str, newstr, start);
    else
    {
        if (**str == '?')
            question(str, newstr, start);
        else
            glue_var(str, newstr, start, info);
    }
}

void check_syms(char **str, t_info *info, char **newstr, char **start)
{
    char *glue;

    while (**str && **str != '\'' && **str != '\"' && **str != '$')
        (*str)++;
    if (**str == '\"')
    {
        if (info->s_quote == 0)
            info->d_quote = !(info->d_quote);
        (*str)++;
    }
    else if (**str == '\'')
    {
        if (info->d_quote == 0)
            info->s_quote = !(info->s_quote);
        (*str)++;
    }
    else if (**str == '$')
    {
        dollar(str, newstr, start, info);
        *start = *str;
    }
}

char *replace_vars(char *str, t_info *info)
{
    int i;
    char *newstr;
    char *start;
    char *glue;
    char *tmp;

    info->d_quote = 0;
    info->s_quote = 0;
    start = str;
    newstr = (char *)malloc(1);
    *newstr = '\0';
    while (*str)
    {
        check_syms(&str, info, &newstr, &start);
        if (*str == '\0')
        {
            glue = ft_substr(start, 0, str - start);
            tmp = newstr;
            newstr = ft_strjoin(tmp, glue);
            free(tmp);
            free(glue);
        }
    }
    return newstr;
}
