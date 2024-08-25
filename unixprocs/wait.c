/*

File:
    wait.c

Description:
    Waiting for a forked child process to finish

Note:
    There's also waitpid function

*/

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t id = fork();

    pid_t n = 0;

    if (id == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    }

    // - wait until one child process is finished executing the rest of the code.
    // - Note that it only wait for one child and resume the process after that.
    // - If there are multiple child processes then it should wait that many times.
    // - Note that the wait system call frees up the resources that the child is using once the
    //   child state changes, i.e. when the child terminates. Without the wait call although the
    //   child will still terminate, nothing would free up its resources, which may lead to memory
    //   issues. The child process that has terminated but still takes up resources (as a result of
    //   not being waited on) is referred to as a zombie process.
    if (id != 0)
    {
        (void) wait(NULL);
    }

    for (int i = n; i < n + 5; i++)
    {
        printf("%d ", i);
        fflush(stdout);
    }

    if (id != 0)
    {
        printf("\n");
    }

    return 0;
}
