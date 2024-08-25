/*
File: sum-1.c

Desc:
    Sum of array of integers with two process. Each process computes the partial sum of half of the
    array. Finally the child process will send its partial sum to the parent process through a pipe
    and the parent process prints the total sum to the screen.
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARR_LEN(ARR) sizeof((ARR)) / sizeof((ARR[0]))
#define FATAL_ERR(MSG, EXITCODE)                                                                   \
    fprintf(stderr, MSG ": [%d] %s\n", errno, strerror(errno));                                    \
    exit((EXITCODE))

int main(void)
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    int arr_size = ARR_LEN(arr);

    // Creating pipe before fork for later use
    int fds[2] = {0};
    int pipeerr = pipe(fds);
    if (pipeerr == -1)
    {
        FATAL_ERR("An error occured with opening pipe", 1);
    }

    // Forking the main process
    pid_t id = fork();
    if (id == -1)
    {
        FATAL_ERR("An error occured with fork", 2);
    }

    // Start and end index of the array for each process (to distribute items in the array)
    int start = {0};
    int end = {0};
    if (id == 0)
    {
        start = 0;
        end = arr_size / 2;
    }
    else
    {
        start = arr_size / 2;
        end = arr_size;
    }

    // Compute the partial sum in each process
    int par_sum = {0};
    for (int i = start; i < end; i++)
    {
        par_sum += arr[i];
    }

    // Print partial results
    if (id == 0)
    {
        printf("[CHILD]  computed partial sum: %d.\n", par_sum);
    }
    else
    {
        printf("[PARENT] computed partial sum: %d.\n", par_sum);
    }

    // Send the result from child to parent
    if (id == 0)
    {
        // No read operation in the child process
        close(fds[0]);
        ssize_t writeerr = write(fds[1], &par_sum, sizeof(par_sum));
        if (writeerr == -1)
        {
            close(fds[1]);
            FATAL_ERR("An error occured with write in the child process.", 3);
        }
        close(fds[1]);
    }
    // Receive the reults in the parent processes and compute the total sum
    else
    {
        // No write operation in parent process
        close(fds[1]);

        // Get the sum from child process
        int sum_from_child = {0};
        ssize_t readerr = read(fds[0], &sum_from_child, sizeof(sum_from_child));
        if (readerr == -1)
        {
            close(fds[0]);
            FATAL_ERR("An error occured with write in the child process.", 4);
        }

        // Compute the total sum
        int total_sum = par_sum + sum_from_child;
        printf("Total sum: %d.\n", total_sum);

        // wait
        wait(NULL);
    }

    return 0;
}
