#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int fd[2][2];
    if (pipe(fd[0]) == -1)
        return (-1);

    int pid1 = fork();
    if (pid1 == 0)
    {
        //child process for command before the pipe
        dup2(fd[0][1], STDOUT_FILENO);
        close(fd[0][0]);
        close(fd[0][1]);
        execlp("cat", "cat", "pipe.c", NULL);
    }

    if (pipe(fd[1]) == -1)
        return (-1);

    int pid2 = fork();
    if (pid2 == 0)
    {
        //child process for command after the pipe
        dup2(fd[0][0], STDIN_FILENO);
        dup2(fd[1][1], STDOUT_FILENO);
        close(fd[0][0]);
        close(fd[0][1]);
        execlp("head", "head", NULL);
    }
    close(fd[0][0]);
    close(fd[0][1]);

    

    int pid3 = fork();
    if (pid3 == 0)
    {
        // dup2(fd[1][1], STDOUT_FILENO);
        dup2(fd[1][0], STDIN_FILENO);
        close(fd[1][0]);
        close(fd[1][1]);
        execlp("grep", "grep", "include", NULL);
    }

    close(fd[0][0]);
    close(fd[0][1]);
    close(fd[1][1]);
    close(fd[1][0]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    
    return 0;
}
