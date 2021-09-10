#include "minishell.h"

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

void add_from_the_first(t_info *info, int q, char **array, int *k)
{
    int q2 = 0;
    int files = 1;
    int i = info->i2;

    while (info->tokens[i].str && info->tokens[i].type != 'p')
    {
        if (info->tokens[i].type == 'l')
        {
            if (q == q2)
            {
                while (info->tokens[i].args[files] && info->tokens[i].args[0])
                {
                    array[*k] = ft_strdup(info->tokens[i].args[files]);
                    (*k)++;
                    files++;
                }
                return;
            }
            q2++;
        }
        i++;
    }
}

void add_redirect_args(char **array, int *k, t_info *info, int smb)
{
    int q;
    
    q = 0;
    while (q < smb)
    {
        add_from_the_first(info, q, array, k);
        q++;
    }
}

char **create_new_exec_array(t_info *info, int files, int smb)
{
    char **array;
    int k;

    k = 0;
    while (info->tokens[info->i].args[k])
        k++;
    array = (char **)malloc(sizeof(char *) * (k + files + 1));
    k = 0;
    while (info->tokens[info->i].args[k])
    {
        array[k] = ft_strdup(info->tokens[info->i].args[k]);
        k++;
    }
    add_redirect_args(array, &k, info, smb);
    array[k] = NULL;
    k = 0;
    while (info->tokens[info->i].args[k])
    {
        free(info->tokens[info->i].args[k]);
        k++;
    }
    free(info->tokens[info->i].args);
    return (array);
}

void prepare_args_and_fd(t_info *info)
{
    int files = 0;
    int smb;
    int q;

    q = 0;
    smb = count_redir(info);
    while (q < smb)
    {
        files += count_files(info, q);
        q++;
    }
    if (smb > 0 && files > 1)
        files = files - smb;
    if (smb)
        info->tokens[info->i].args = create_new_exec_array(info, files, smb);
    define_fd_out(info);
    define_fd_in(info);
}

int exec_printable_buildin(t_info *info)
{
    start_of_line(info);
    if (ft_strlen(info->tokens[info->i].str) == 3 && !ft_strncmp(info->tokens[info->i].str, "pwd", 3))
    {
        pwd(info);
        return(1);
    }
    else if (ft_strlen(info->tokens[info->i].str) == 4 && !ft_strncmp(info->tokens[info->i].str, "echo", 4))
    {
        echo(info);
        return (1);
    }
    else if (ft_strlen(info->tokens[info->i].str) == 3 && !ft_strncmp(info->tokens[info->i].str, "env", 3))
    {
        env(info);
        return(1);
    }
    else if (ft_strlen(info->tokens[info->i].str) == 6 && !ft_strncmp(info->tokens[info->i].str, "export", 6))
    {
        export(info);
        return(1);
    }
    return(0);
}
void exec_printable(t_info *info, char *cmd)
{
    char **envp_arr;
    int buildin;

    buildin = exec_printable_buildin(info);
    if (!buildin && cmd)
    {
        if (info->result)
            free(info->result);
        info->result = NULL;
        prepare_args_and_fd(info);
        envp_arr = make_envp_arr(info);
        execve(cmd, info->tokens[info->i].args, envp_arr);
        free(envp_arr);
    }
    else if (!buildin)
    {
        write(2, "dashBash: ", 11);
        write(2, info->tokens[info->i].str, ft_strlen(info->tokens[info->i].str));
        write(2, ": command not found\n", 21);
        info->result = NULL;
        exit(127);
    }
}

int set_start(t_info *info)
{
    int i;

    i = 0;
    while (info->tokens[i].str && info->tokens[i].type != 'p')
    {
        if (info->tokens[i].type == 'c')
        {
            info->i = i;
            return 1;
        }
        i++;
    }
    return (0);
}
