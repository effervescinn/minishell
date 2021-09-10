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
    int i = info->i;
    int q = 0;
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
    int flag = 0;

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
    char *shlvl = NULL;
    int buildin;

    buildin = exec_printable_buildin(info);
    if (!buildin && cmd)
    {
        if (info->result)
            free(info->result);
        info->result = NULL;
        prepare_args_and_fd(info);
        envp_arr = make_envp_arr(info, &shlvl);
        execve(cmd, info->tokens[info->i].args, envp_arr);
        if (shlvl)
            free(shlvl);
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

void program_define(t_info *info)
{
    char *cmd;
    pid_t pid;
    int fd_dasha;
    int *pids;
    int **fd;
    int k = -1; //счетчик для пайпов
    int j;

    pids = (int *)malloc(sizeof(int) * (info->pipes_num + 1));
    fd = (int **)malloc(sizeof(int *) * info->pipes_num);
    j = 0;
    while (j < info->pipes_num)
    {
        fd[j] = (int *)malloc(sizeof(int) * 2);
        j++;
    }
    info->result = ft_strdup("\0");
    search_heredoc(info);

    while (++k < info->pipes_num + 1)
    {

        if (k < info->pipes_num)
            if (pipe(fd[k]) < 0)
                return;
        if (info->tokens[info->i].type != 'c' && info->tokens[info->i].type != 'L')
        {
            if (info->tokens[info->i + 2].str && info->tokens[info->i + 2].type == 'c')
                info->i += 2;
            else
            {
                redirects_solo(info);
                if (info->tokens[info->i].str && info->tokens[info->i + 1].str)
                    info->i += 1;
                continue;
            }
        }

        if (info->tokens[info->i].print == 0)
        {
            if (k == 0 && info->pipes_num == 0)
                exec_builtin(info);
            else
            {
                (info->i)++;
                while (info->tokens[info->i].str && info->tokens[info->i].type != 'p')
                    (info->i)++;
                (info->i)++;
                if (k < info->pipes_num)
                    write(fd[k][1], "\0", 1);
                continue;
            }
        }
        else
        {
            j = 0;
            char *cmd;
            cmd = find_bin(info);
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
                exec_printable(info, cmd);
                if (info->result)
                {
                    fd_dasha = define_fd_built_in(info);
                    write(fd_dasha, info->result, ft_strlen(info->result));
                    free(info->result);
                    info->result = NULL;
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
                info->i2 = info->i;
                exec_printable(info, cmd);
                if (info->result)
                {
                    fd_dasha = define_fd_built_in(info);
                    write(fd_dasha, info->result, ft_strlen(info->result));
                    free(info->result);
                    info->result = NULL;
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
                exec_printable(info, cmd);

                if (info->result)
                {
                    fd_dasha = define_fd_built_in(info);
                    write(fd_dasha, info->result, ft_strlen(info->result));
                    free(info->result);
                    info->result = NULL;
                }
                exit(0);
            }
            free(cmd);
        }
        while (info->tokens[info->i].str && info->tokens[info->i].type != 'p')
            (info->i)++;
        (info->i)++;
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
    unlink_files(info);

    if (info->result)
    {
        free(info->result);
        info->result = NULL;
    }
    (info->i) = 0;

    free(pids);
    j = 0;
    while (j < info->pipes_num)
    {
        free(fd[j]);
        j++;
    }
    free(fd);
}
