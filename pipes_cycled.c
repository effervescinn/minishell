#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "signal.h"
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    int num; //number of pipes
    int i;
    int *pids;
    int **fd;
    int j;

    num = 2;
    fd = (int **)malloc(sizeof(int *) * num);
    pids = (int *)malloc(sizeof(int) * (num + 1));

    i = 0;
    while (i < num)
    {
        fd[i] = (int *)malloc(sizeof(int) * 2);
        i++;
    }

    i = 0;
    while (i < num + 1)
    {
        j = 0;
        if (i < num)
            if (pipe(fd[i]) < 0)
                return (-1);

        pids[i] = fork();
        if (i == 0 && pids[i] == 0)
        {
            if (num != 0)
            {
                dup2(fd[i][1], STDOUT_FILENO);
                close(fd[i][0]);
                close(fd[i][1]);
            }
            execlp("echo", "echo", "hello world", NULL);
            return (0);
        }
        else if (i != num && pids[i] == 0)
        {
            dup2(fd[i - 1][0], STDIN_FILENO);
            dup2(fd[i][1], STDOUT_FILENO);
            while (j <= i)
            {
                close(fd[j][0]);
                close(fd[j][1]);
                j++;
            }
            execlp("cat", "cat", "-e", NULL);
        }
        else if (i == num && pids[i] == 0)
        {
            dup2(fd[i - 1][0], STDIN_FILENO);
            while (j <= i - 1)
            {
                close(fd[j][0]);
                close(fd[j][1]);
                j++;
            }
            execlp("grep", "grep", "hello", NULL);
        }

        i++;
    }

    i = 0;
    while (i < num)
    {
        close(fd[i][0]);
        close(fd[i][1]);
        i++;
    }

    i = 0;
    while (i < num + 1)
    {
        waitpid(pids[i], NULL, 0);
        i++;
    }
    return 0;
}
