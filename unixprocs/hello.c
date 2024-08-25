/*
File:
    hello.c

Desc:
    Prints hello from different processes.

         proc
       [a=1,b=2]
        /    \
       /      \
      /        \
    child     parent
  [a=1,b=2]  [a=1,b=2]

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    // Clone the calling process, creating an exact copy.
    // Return:
    // - pid = -1 on error (in the parent process),
    // - pid = 0 in the child process,
    // - pid = <pid> in the parent process (<pid> is the process id of the child process)
    //         <pid> is usually <parent-pid> + 1
    //
    // pid_t: some os-dependent integer (on macos: int)
    // typedef int                     __int32_t;
    // typedef __int32_t       __darwin_pid_t;         /* [???] process and group IDs */
    // typedef __darwin_pid_t        pid_t;
    pid_t pid = fork();

    printf("Hello from proc: %d\n", pid);

    if (pid == 0)
    {
        // Code executed only in child proc
        printf("Hello again from child proc (pid = 0)!\n");
    }

    // Wait for process termination in the calling process (also frees up resources of the child
    // memory)
    if (pid != 0)
    {
        wait(NULL);
    }

    return 0;
}
