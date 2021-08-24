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

    num = 1;
    
    fd = (int**)malloc(sizeof(int*) * num);
    pids = (int*)malloc(sizeof(int) * (num + 1));
    i = 0;
    while (i < num)
    {
        fd[i] = (int*)malloc(sizeof(int) * 2);
        i++;
    }
    i = 0;
    while (i < num + 1)
    {
        if (i < num)
            if (pipe(fd[i]) < 0)
                return (-1);
        
        pids[i] = fork();
        if (i == 0 && pids[i] == 0)
        {
            dup2(fd[i][1], STDOUT_FILENO);
            close(fd[i][0]);
            close(fd[i][1]);
            execlp("echo", "echo", "hello", NULL);
        }

        else if (i == (num) && pids[i] == 0)
        {
            dup2(fd[i - 1][0], STDIN_FILENO);
            close(fd[i - 1][0]);
            close(fd[i - 1][1]);
            execlp("cat", "cat", "-e", NULL);
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
