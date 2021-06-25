#include "libft/libft.h"
#include "stdio.h"

//lexer
typedef struct s_command
{
    char *name;
    char **arguments;
    int pipe;     //0,1
    int redirect; //1, 2, 3, 4?
} t_command;

typedef struct s_token
{
    char *token;
    char *type; //word or command or argument
} t_token;

///////////////////////changing env vars

int count_len(char *str)
{
    int len;

    len = 0;
    while (*str)
    {
        len++;
        str++;
    }
    return (len);
}

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
        if (head->content[j] == '=' && ((*str)[j] == ' ' || (*str)[j] == '\0'))
        {
            j++;
            newvar = ft_substr(&(head->content[j]), 0, count_len(&(head->content[j])));
            str += j;
            return (newvar);
        }
        head = head->next;    
    }
    if (newvar == NULL)
    {
        newvar = (char *)malloc(1);
        *newvar = '\0';
    }
    return (newvar);
}

void dollar(char **str, char **newstr, char **start, t_list *head)
{
    char *tmp;
    char *glue;
    char *var;

    (*str)++;
    if (**str == ' ' || **str == '\0')
    {
        tmp = *newstr;
        *newstr = ft_strjoin(tmp, "$");
        if (tmp)
            free(tmp);
        *start = *str;
        return;
    }
    else
    {
        var = vars(str, head);
        glue = ft_substr(*start, 0, *str - *start - 1);
        tmp = *newstr;
        *newstr = ft_strjoin(tmp, glue);
        free(tmp);
        free(glue);
        tmp = *newstr;
        *newstr = ft_strjoin(tmp, var);
        free(tmp);
        free(var);
        while (**str != ' ' && **str != '\0')
            (*str)++;
        *start = *str;
        return;
    }
}

void quotes(char **str, char **newstr, char **start, t_list *head)
{
    int change;
    char *glue;
    char *tmp;
    char *var;
    int dquote;

    dquote = 0;
    if (**str == '$')
    {
        // dollar(str, newstr, start, head);
        (*str)++;
        if (**str == ' ' || **str == '\0')
        {
            tmp = *newstr;
            *newstr = ft_strjoin(tmp, "$");
            free(tmp);
            *start = *str;
            return;
        }
        else
        {
            var = vars(str, head);
            glue = ft_substr(*start, 0, *str - *start - 1);
            tmp = *newstr;
            // printf("|%s|\n", glue);
            *newstr = ft_strjoin(tmp, glue);
            free(tmp);
            free(glue);
            tmp = *newstr;
            *newstr = ft_strjoin(tmp, var);
            free(tmp);
            free(var);
            while (**str != ' ' && **str != '\0')
                (*str)++;
            *start = *str;
            return;
        }
    }
    else if (**str == '\'')
    {
        (*str)++;
        while (**str && **str != '\'')
            (*str)++;
        glue = ft_substr(*start, 0, *str - *start + 1);
        tmp = *newstr;
        *newstr = ft_strjoin(tmp, glue);
        if (tmp)
            free(tmp);
        if (glue)
            free(glue);
        *start = ++(*str);
    }
    // else //echo "$PWD hello"
    // {
    //     dquote = != dquote;

    // }
}

char *replace_vars(char *str, t_list *head) //потом почистить str после замены
{
    int i;
    int len;
    char *newstr;
    char *start;
    char *glue;
    char *tmp;

    i = 0;
    start = str;
    newstr = (char *)malloc(1);
    *newstr = '\0';

    while (*str)
    {
        while (*str && *str != '\'' && *str != '\"' && *str != '$')
            str++;
        if (*str == '\0')
        {
            glue = ft_substr(start, 0, str - start);
            tmp = newstr;
            newstr = ft_strjoin(tmp, glue);
            free(tmp);
        }
        quotes(&str, &newstr, &start, head);
    }

    return (newstr);
}

///////////////////////

void add_export(t_list **head, char *addition)
{
    t_list *new;

    new = (t_list *)malloc(sizeof(t_list));
    new->content = addition;
    new->next = NULL;
    ft_lstadd_back(head, new);
    //sort_env();
}

void make_env(char **envp, t_list **head)
{

    t_list *new;
    int i;

    i = 1;
    *head = ft_lstnew(envp[0]);
    while (envp[i])
    {
        new = (t_list *)malloc(sizeof(t_list));
        new->content = envp[i];
        new->next = NULL;
        ft_lstadd_back(head, new);
        i++;
    }
}

int main(int argc, char **argv, char **envp)
{
    t_list *head;
    make_env(envp, &head);
    char *str = "'  $be'";
    char *newstr;
    newstr = replace_vars(str, head);
    printf("%s\n", newstr);
    return 0;
}
