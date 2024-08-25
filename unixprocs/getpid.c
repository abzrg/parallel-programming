#include <stdio.h>
#include <sys/wait.h> // getpid, getppid
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();

    // If parent finishes early the parent id of the child will be set to the pid of the init
    // process (e.g. pid of systemd in Linux which is 1)
#ifdef PARENT_FINISH_EARLY
    if (pid == 0)
    {
        sleep(3);
    }

    /*

    $ ./getpid
    parent proc id = 22427
    parent's parent proc id = 15663
    $ child proc id = 22428
    child's parent proc id = 1                   <- Note this

    */
#endif

    pid_t procid = getpid();
    pid_t pprocid = getppid();

    if (pid == 0)
    {
        printf("child proc id = %d\n", procid);
        printf("child's parent proc id = %d\n", pprocid);
    }
    else
    {
        printf("parent proc id = %d\n", procid);
        printf("parent's parent proc id = %d\n", pprocid);
    }

    // If parent finishes early the child process becomes a zombie process whose parent is the init
    // system's process id. This zombie process does not free the memory with SIGKILL signal.
#ifndef PARENT_FINISH_EARLY
    printf("\n");
    // Returns the process id of the child process that it waits for
    // Note that we don't need to check if we are in the parent process to call wait.
    // Calling wait in both processes works properly as well.
    pid_t waiterr = wait(NULL);
    if (waiterr == -1)
    {
        printf("[ CHILD ] No child to wait for.\n");
    }
    else
    {
        printf("[ PARENT ] %d finished execution.\n", waiterr);
    }
#endif

    return 0;
}
