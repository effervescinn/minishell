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

// char *glue(char *newstr, char *start, char *end, char *env)
// {
//     char *temp;
//     char *before;

//     temp = newstr;
//     before = ft_substr(newstr, start, end - start);
//     newstr = ft_strjoin(before, env);
//     free(temp);
//     return(newstr);
// }

// char *str = "e\"ch\"o    '$PWD    '     >> test.txt";


// void find_vars(char **str)
// {

// }

//  "echo '$PWD\"\" ' $ hah"

///////////////////////changing env vars

void quotes(char **str, char **newstr, char **start)
{
    int dquote;
    int quote;
    int change;
    char *glue;
    char *tmp;

    dquote = 0;
    quote = 0;
    if (**str == '$')
    {
        (*str)++;
        if (**str == ' ' || **str == '\0')
        {
            tmp = *newstr;
            *newstr = ft_strjoin(tmp, "$");
            free(tmp);
            *start = *str;
            return ;
        }
        else
        {

        }
    }
    if (**str == '\'')
    {
        (*str)++;
        while (**str && **str != '\'')
            (*str)++;
        if (**str == '\0')
            return;
        else
        {
            glue = ft_substr(*start, 0, *str - *start + 1);
            tmp = *newstr;
            *newstr = ft_strjoin(tmp, glue);
            free(tmp);
            *start = ++(*str);
            
        }
    }

    // if (str[i] == '\"')
    // {
    //     dquote = !dquote
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
    newstr = malloc(1);
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
        quotes(&str, &newstr, &start);
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
    char *str = "echo '$PWD\"\" ' $ hah";
    char *newstr;
    newstr = replace_vars(str, head);
    printf("%s\n", newstr);
    // printf("%s\n", str);
    return 0;
}
