/*

Description:
   Demonstration of calling fork inside of another fork.

Advice:
   1. Don't call fork in a loop
   2. Call fork in the parent process only

Note:
    The wait function only waits for one (immediate) child process to finish. One have to call wait
    multiple times for all the child processes. In the following example, the parent process should
    wait two times and the process x should wait once.

ASCII Art:

        p
    +--------------+
    |    parent    |
    |    id1 = x   |---------+
    |    id2 = z   |         |
    +--------------+         |
          |                  |
      x   |              z   |
     +---------+        +---------+
     | child x |        | child z |
     | id1 = 0 |        | id1 = x |
     | id2 = y |        | id2 = 0 |
     +---------+        +---------+
          |
      y   |
     +---------+
     | child y |
     | id1 = 0 |
     | id2 = 0 |
     +---------+

*/

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t id1 = fork();
    pid_t id2 = fork();

    if (id1 == 0)
    {
        if (id2 == 0)
        {
            printf("We are process y.\n");
        }
        else
        {
            printf("We are process x.\n");
        }
    }
    else
    {
        if (id2 == 0)
        {
            printf("We are process z.\n");
        }
        else
        {
            printf("We are the parent process.\n");
        }
    }

    // Wait for all child processes to finish (wait 3 times)
    int status = {0};
    pid_t wpid = {0};
    while (true)
    {
        wpid = wait(&status);

        if (wpid != -1 && status == 0)
        {
            printf("Waited for a child to finish successfully.\n");
        }
        else if (wpid != -1 && status != 0)
        {
            fprintf(stderr, "Waited for a child to finish with error: [%d] %s .\n", errno,
                    strerror(errno));
        }
        else
        {
            break;
        }
    }

    // or

    /*

    while ((wpid = wait(&status)) != -1 || errno != ECHILD)
    {
        printf("Waited for a child to finish\n");
    }

    */
}
