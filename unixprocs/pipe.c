/*
File:
    pipe.c

Desc:
    Communication between two processes using a pipe. In this example code, the child process reads
    an integer provided by the user and sends it (through a pipe) to the parent process to print it
    to the screen.

    Pipes are sort of 'in-memory' files that we can be write to and read from them.

Note:
    Always check for error codes returned by function calls
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(void)
{
    // A pipe has two file descriptors
    // fd[0] - read
    // fd[1] - write
    int fildes[2] = {0}; // or 'int fd[2]'

    // Note that we create the pipe before forking the main process to have the child process
    // inherting the file descriptors.
    // Note that each process has to manage (close) its own pair of file descriptors (4 file
    // descriptors in total)
    int pipeerr = pipe(fildes);
    if (pipeerr == -1)
    {
        fprintf(stderr, "An error occurred with opening the pipe.\n");
        exit(1);
    }

    pid_t id = fork();
    if (id == -1)
    {
        fprintf(stderr, "An error occurred with fork.\n");
        exit(2);
    }

    // In the child process send the user-provided integer to the parent process
    if (id == 0)
    {
        // In the child process no read operation is done
        close(fildes[0]);

        // Read an integer from user
        int x = {0};
        printf("Enter a number: ");
        scanf("%d", &x);

        // Write the integer to the pipe created above
        ssize_t writeerr = write(fildes[1], &x, sizeof(x));
        if (writeerr == -1)
        {
            fprintf(stderr, "An error occurred with writing to pipe.\n");
            exit(3);
        }
        // Now we have no business with write end of the of the pipe and we release the resource
        // related to that.
        close(fildes[1]);
    }
    else
    {
        // We don't need to write to pipe in parent process
        close(fildes[1]);

        int y = {0};

        ssize_t readerr = read(fildes[0], &y, sizeof(y));
        if (readerr == -1)
        {
            fprintf(stderr, "An error occurred with reading from pipe.\n");
            exit(4);
        }

        // We are done with reading from the pipe
        close(fildes[0]);

        // Manipulate the read data
        y *= 3;

        printf("Got the value of %d from the child process.\n", y);
    }

    wait(NULL);

    return 0;
}
