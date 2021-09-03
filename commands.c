#include "minishell.h"

void copy_pwds(t_info *info)
{
    if (info->pwd)
        info->str_pwd = ft_strdup(info->pwd->content);
    if (info->oldpwd)
        info->str_oldpwd = ft_strdup(info->oldpwd->content);
}

void set_pointers(t_info *info)
{
    t_list *tmp;

    info->pwd = NULL;
    info->oldpwd = NULL;
    info->pths = NULL;
    tmp = info->head;
    while (tmp)
    {
        if (!ft_strncmp(tmp->content, "OLDPWD=", 7) || !ft_strncmp(tmp->content, "OLDPWD\0", 7))
            info->oldpwd = tmp;
        if (!ft_strncmp(tmp->content, "PWD=", 4) || !ft_strncmp(tmp->content, "PWD\0", 4))
            info->pwd = tmp;
        else if (!ft_strncmp(tmp->content, "PATH=", 5))
            info->pths = tmp;
        tmp = tmp->next;
    }
}

void swap_content(t_list *list1, t_list *list2)
{
    char *tmp;

    tmp = list1->content;
    list1->content = list2->content;
    list2->content = tmp;
}

void export_order(t_info *info, int i)
{
    t_list *next;
    t_list *tmp;
    int dif;
    t_list *head;

    head = info->exp;
    while (i--)
    {
        while (info->exp->next)
        {
            next = info->exp->next;
            dif = ft_strncmp(info->exp->content, next->content, ft_strlen(info->exp->content));
            if (dif > 0)
                swap_content(info->exp, next);
            info->exp = info->exp->next;
        }
        info->exp = head;
    }
}

void make_exp(t_info *info)
{
    t_list *env_tmp;
    t_list *exp_tmp;
    t_list *extra_tmp;
    int i;

    i = 1;
    env_tmp = info->head;
    info->exp = ft_lstnew(ft_strdup(info->head->content));
    exp_tmp = info->exp;
    while (env_tmp->next)
    {
        env_tmp = env_tmp->next;
        exp_tmp->next = malloc(sizeof(t_list));
        exp_tmp = exp_tmp->next;
        exp_tmp->content = ft_strdup(env_tmp->content);
        i++;
    }
    if (!info->extra_exp)
        exp_tmp->next = NULL;
    else
    {
        extra_tmp = info->extra_exp;
        while (extra_tmp)
        {
            exp_tmp->next = malloc(sizeof(t_list));
            exp_tmp = exp_tmp->next;
            exp_tmp->content = ft_strdup(extra_tmp->content);
            extra_tmp = extra_tmp->next;
            i++;
        }
        exp_tmp->next = NULL;
    }
    export_order(info, i);
}

char *no_leaks_join(char *str1, char *str2)
{
    char *tmp;

    tmp = str1;
    str1 = ft_strjoin(str1, str2);
    free(tmp);
    return (str1);
}

void echo(t_info *info)
{
    int n;
    int a;

    n = 0;
    a = 1;
    if (!info->tokens[info->i].args[a])
    {
        info->result = ft_strdup("\n\0");
        return;
    }
    if (ft_strlen(info->tokens[info->i].args[a]) == 2 && !ft_strncmp(info->tokens[info->i].args[a], "-n", 2))
    {
        a++;
        if (!info->tokens[info->i].args[a])
        {
            info->result = ft_strdup("\0");
            return;
        }
        n = 1;
    }
    while (info->tokens[info->i].args[a])
    {
        info->result = no_leaks_join(info->result, info->tokens[info->i].args[a]);
        if (info->tokens[info->i].args[a + 1])
            info->result = no_leaks_join(info->result, " ");
        a++;
    }
    if (!n)
        info->result = no_leaks_join(info->result, "\n");
}

void pwd(t_info *info)
{
    char *buf;

    buf = getcwd(NULL, 100);
    if (buf)
    {
        info->result = no_leaks_join(info->result, buf);
        free(buf);
        info->result = no_leaks_join(info->result, "\n");
    }
    else
        exit(0);
}

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

char *add_quotes(char *str)
{
    char *result;
    int i;

    i = 0;
    result = malloc(sizeof(char) * ft_strlen(str) + 3);
    while (str[i] && str[i] != '=')
    {
        result[i] = str[i];
        i++;
    }
    result[i] = '=';
    result[++i] = '"';
    i++;
    while (str[i - 1])
    {
        result[i] = str[i - 1];
        i++;
    }
    result[i] = '"';
    result[i + 1] = '\0';
    free(str);
    return (result);
}

void free_list(t_list **list)
{
    t_list *tmp;

    while (*list)
    {
        tmp = *list;
        *list = (*list)->next;
        free(tmp->content);
        free(tmp);
    }
    *list = NULL;
}

char *up_dir(char *str)
{
    char *new;
    int i;
    int r;

    i = 0;
    while (str[i])
    {
        if (str[i] == '/')
            r = i;
        i++;
    }
    new = malloc(sizeof(char) * r + 1);
    i = 0;
    while (i < r)
    {
        new[i] = str[i];
        i++;
    }
    new[r - 1] = '\0';
    free(str);
    return (new);
}

void add_oldpwd(t_info *info)
{
    t_list *tmp;

    if (!info->oldpwd_flag)
    {
        tmp = info->head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = malloc(sizeof(t_list));
        tmp = tmp->next;
        tmp->content = ft_strdup("OLDPWD=");
        tmp->next = NULL;
        info->oldpwd_flag = 1;
        set_pointers(info);
    }
}

void new_pwd_frst(t_info *info)
{
    add_oldpwd(info);
    if (info->oldpwd)
    {
        if (info->str_oldpwd)
        {
            free(info->str_oldpwd);
            info->str_oldpwd = NULL;
            if (info->pwd)
                info->str_oldpwd = ft_strdup(info->pwd->content);
        }
        if (info->oldpwd)
        {
            free(info->oldpwd->content);
            if (info->str_pwd)
                info->oldpwd->content = ft_strjoin("OLD", info->str_pwd);
            else
                info->oldpwd->content = ft_strdup("OLDPWD=");
        }
    }
}

void new_pwd_scnd(t_info *info, char *buf)
{
    if (info->str_pwd)
        free(info->str_pwd);
    info->str_pwd = ft_strjoin("PWD=", buf);
    if (info->pwd)
    {
        free(info->pwd->content);
        info->pwd->content = ft_strdup(info->str_pwd);
    }
    free(buf);
}

void cd(t_info *info)
{
    char *buf;
    char *tmp2;
    int a;

    if (!info->tokens[info->i].args[1])
        return;
    a = chdir(info->tokens[info->i].args[1]);
    if (!a)
    {
        new_pwd_frst(info);
        buf = getcwd(NULL, 100);
        if (buf && info->pwd)
            new_pwd_scnd(info, buf);
        else if (!buf)
        {
            while (!buf)
            {
                free(info->tokens[info->i].args[1]);
                info->tokens[info->i].args[1] = ft_strdup(info->str_pwd + 4);
                info->tokens[info->i].args[1] = up_dir(info->tokens[info->i].args[1]);
                a = chdir(info->tokens[info->i].args[1]);
                if (!a)
                {
                    new_pwd_frst(info);
                    buf = getcwd(NULL, 100);
                    if (buf && info->pwd)
                        new_pwd_scnd(info, buf);
                }
            }
        }
    }
    else
        opening_error(info->tokens[info->i].args[1]);
}

void remove_var(t_info *info, t_list **list, int a)
{
    t_list *tmp;
    int var_len;
    t_list *prev;

    tmp = *list;
    var_len = ft_strlen(info->tokens[info->i].args[a]);
    while (tmp)
    {
        if (!ft_strncmp(tmp->content, info->tokens[info->i].args[a], var_len) && tmp->content[var_len] == '=')
        {
            if (tmp == *list)
            {
                *list = tmp->next;
                free(tmp->content);
                free(tmp);
                return;
            }
            else
            {
                prev->next = tmp->next;
                ft_lstdelone(tmp, free);
                return;
            }
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

void unset(t_info *info)
{
    int i;
    int a;

    a = 1;
    while (info->tokens[info->i].args[a])
    {
        i = 0;
        while (info->tokens[info->i].args[a][i])
        {
            if (!ft_isalpha(info->tokens[info->i].args[a][i]) && !ft_isdigit(info->tokens[info->i].args[a][i]))
            {
                write(2, "-dashBash: unset: `", 19);
                write(2, info->tokens[info->i].args[a], ft_strlen(info->tokens[info->i].args[a]));
                write(2, "': not a valid identifier\n", 26);
                return;
            }
            i++;
        }
        remove_var(info, &info->head, a);
        remove_from_extra_exp(&info->extra_exp, info->tokens[info->i].args[a]);
        if ((!ft_strncmp(info->tokens[info->i].args[a], "PATH", 4) && ft_strlen(info->tokens[info->i].args[a]) == 4)
            || (!ft_strncmp(info->tokens[info->i].args[a], "PWD", 3) && ft_strlen(info->tokens[info->i].args[a]) == 3)
            || (!ft_strncmp(info->tokens[info->i].args[a], "OLDPWD", 6) && ft_strlen(info->tokens[info->i].args[a]) == 6))
            {
                set_pointers(info);
                make_paths(info);
            }
        a++;
    }
}

void exit_minishell(t_info *info)
{
    write(1, "exit\n", 6);
    if (info->tokens[info->i + 1].str)
        {
            write(2, "-dashBash: exit: ", 17);
            write(2, info->tokens[info->i + 1].str, ft_strlen(info->tokens[info->i + 1].str));
            write(2, ": numeric argument required\n", 28);
        }
    free_list(&info->extra_exp);
    free_list(&info->head);
    free_args(info);
    free_tokens(info);
    free(info->str_pwd);
    free(info->str_oldpwd);
    free_paths_array(info);
    exit(1);
}

void exec_builtin(t_info *info)
{
    if (!ft_strncmp(info->tokens[info->i].str, "cd", 2) && ft_strlen(info->tokens[info->i].str) == 2)
        cd(info);
    else if (ft_strlen(info->tokens[info->i].str) == 5 && !ft_strncmp(info->tokens[info->i].str, "unset", 5))
        unset(info);
    else if (ft_strlen(info->tokens[info->i].str) == 4 && !ft_strncmp(info->tokens[info->i].str, "exit", 4))
        exit_minishell(info);
    else if (ft_strlen(info->tokens[info->i].str) == 6 && !ft_strncmp(info->tokens[info->i].str, "export", 6))
        export(info);
    g_global.ex_status = 0;
}
void find_command(t_info *info)
{
    while (info->tokens[info->i].str && info->tokens[info->i].type[0] != 'p')
    {
        if (info->tokens[info->i].type[0] == 'c')
            return;
        info->i++;
    }
}

void index_begin(t_info *info)
{
    while (info->i2 && info->tokens[info->i2].type[0] != 'p')
        info->i2--;
    if (info->tokens[info->i2].type[0] != 'p')
        info->i2++;
}

void exec_printable(t_info *info)
{
    char *cmd;

    // info->result = ft_strdup("\0");
    if ((!ft_strncmp(info->tokens[info->i].str, ">>", 2) && ft_strlen(info->tokens[info->i].str) == 2)
        || (!ft_strncmp(info->tokens[info->i].str, ">", 1) && ft_strlen(info->tokens[info->i].str) == 1)
        || (!ft_strncmp(info->tokens[info->i].str, "<", 1) && ft_strlen(info->tokens[info->i].str) == 1))
            find_command(info);
    cmd = find_bin(info);
    if (!ft_strncmp(info->tokens[info->i].str, "<<", 2) && ft_strlen(info->tokens[info->i].str) == 2)
        search_heredoc(info);
    else if (ft_strlen(info->tokens[info->i].str) == 3 && !ft_strncmp(info->tokens[info->i].str, "pwd", 3))
        pwd(info);
    else if (ft_strlen(info->tokens[info->i].str) == 4 && !ft_strncmp(info->tokens[info->i].str, "echo", 4))
        echo(info);
    else if (ft_strlen(info->tokens[info->i].str) == 3 && !ft_strncmp(info->tokens[info->i].str, "env", 3))
        env(info);
    else if (ft_strlen(info->tokens[info->i].str) == 6 && !ft_strncmp(info->tokens[info->i].str, "export", 6))
        export(info);
    else if (cmd)
    {
        int files;
        int smb;
        int q;
        int flag = 0;
        q = 0;

        info->i2 = info->index;
        index_begin(info);
        smb = count_redir(info);
        if (!smb)
            exec_once(info, cmd);
        while (q < smb)
        {
            files = count_files(info);
            if (files == 1 && q + 1 < smb)
            {
                q++;
                info->i2++;
                continue;
            }
            if ((files == 1 || files == 0) && !flag)
                exec_once(info, cmd);
            else
                exec_few_times(&flag, info, cmd, files);
            info->i2++;
            q++;
        }
        // free(cmd);
        free(info->result);
        info->result = NULL;
    }
    else
    {
        write(2, "dashBash: ", 11);
        write(2, info->tokens[info->i].str, ft_strlen(info->tokens[info->i].str));
        write(2, ": command not found\n", 21);
        info->result = NULL;
        exit(127);
    }
    free(cmd);
}

void program_define(t_info *info)
{
    char *cmd;
    pid_t pid;
    int fd_dasha;
    // int *pids; //for pipes
    // int **fd; //for pipes
    int pids[info->pipes_num + 1];
    int fd[info->pipes_num][2];
    int k = 0; //счетчик для пайпов
    int j;

    info->index = info->i;
	// char *tmp = info->result;
    // info->result = malloc(1);
	// if (tmp)
	// 	free(tmp);
    // info->result[0] = '\0';
    if (info->result)
    {
        free(info->result);
        info->result = NULL;
    }
    info->result = ft_strdup("\0");

    while (k < info->pipes_num + 1)
    {

        if (k < info->pipes_num)
            if (pipe(fd[k]) < 0)
                return;

        if (info->tokens[info->i].print == 0)
        {
            if (k == 0 && info->pipes_num == 0)
            {
                exec_builtin(info);
                fd_dasha = define_fd_built_in(info);
                write(fd_dasha, info->result, ft_strlen(info->result));
            }   
            else
            {
                fd_dasha = define_fd_built_in(info);
                write(fd_dasha, info->result, ft_strlen(info->result));
                // free(info->result);

                (info->i)++;
                while (info->tokens[info->i].str && info->tokens[info->i].type[0] != 'c')
                    (info->i)++;
                replace_index(info);
                
                if (k < info->pipes_num)
                    write(fd[k][1], "\0", 1);
                k++;
                continue;
            }
        }
        else
        {
            j = 0;

            pids[k] = fork();
            g_global.f = 1;

            if (k == 0 && pids[k] == 0)
            {
                signal(SIGQUIT, SIG_DFL);
                if (info->pipes_num != 0)
                {
                    dup2(fd[k][1], STDOUT_FILENO);
                    close(fd[k][0]);
                    close(fd[k][1]);
                }
                exec_printable(info);
                if (info->result)
                {
                    fd_dasha = define_fd_built_in(info);
                    write(fd_dasha, info->result, ft_strlen(info->result));
                    free(info->result);
                }
                exit(0);
            }

            else if (k != info->pipes_num && pids[k] == 0)
            {
                dup2(fd[k - 1][0], STDIN_FILENO);
                dup2(fd[k][1], STDOUT_FILENO);
                while (j <= k)
                {
                    close(fd[j][0]);
                    close(fd[j][1]);
                    j++;
                }
                exec_printable(info);
                if (info->result)
                {
                    fd_dasha = define_fd_built_in(info);
                    write(fd_dasha, info->result, ft_strlen(info->result));
                    free(info->result);
                }
                exit(0);
            }
            else if (k == info->pipes_num && pids[k] == 0)
            {
                dup2(fd[k - 1][0], STDIN_FILENO);
                while (j <= k - 1)
                {
                    close(fd[j][0]);
                    close(fd[j][1]);
                    j++;
                }
                info->i2 = info->i;
                exec_printable(info);
                if (info->result)
                {
                    fd_dasha = define_fd_built_in(info);
                    write(fd_dasha, info->result, ft_strlen(info->result));
                    free(info->result);
                }
                exit(0);
            }
        }
        (info->i)++;
        while (info->tokens[info->i].str && info->tokens[info->i].type[0] != 'c')
            (info->i)++;
        replace_index(info);
        k++;
    }
    k = 0;
    while (k < info->pipes_num)
    {
        close(fd[k][0]);
        close(fd[k][1]);
        k++;
    }
    int ex;

    k = 0;
    while (k < info->pipes_num + 1)
    {
        waitpid(pids[k], &ex, 0);
        k++;
    }
    g_global.ex_status = WEXITSTATUS(ex);
    g_global.f = 0;
    if (info->result)
    {
        free(info->result);
        info->result = NULL;
    }
    // info->result = ft_strdup("\0");
    (info->i) = 0;
}
