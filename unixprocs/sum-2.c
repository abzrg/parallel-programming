/*
File: sum-2.c

Desc:
    Same as sum-1.c except to job is done by two child processes and the results are returned back
to the parent process.

Author:
    Ali Bozorgzadeh <https://github.com/abzrg>
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
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18, 19, 20};
    int arr_size = ARR_LEN(arr);

    // First pipe to communicate between the first child and the parent
    // pfds: pipe file descriptors
    int pfds1[2] = {0};
    int pipeerr = pipe(pfds1);
    if (pipeerr == -1)
    {
        FATAL_ERR("An error occured with opening the first pipe", 1);
    }

    // Second pipe to communicate between the second child and the parent
    // pfds: pipe file descriptors
    int pfds2[2] = {0};
    pipeerr = pipe(pfds2);
    if (pipeerr == -1)
    {
        FATAL_ERR("An error occured with opening the second pipe", 1);
    }

    // Creating first child
    pid_t id1 = fork();
    if (id1 == -1)
    {
        FATAL_ERR("An error occured with the first fork", 2);
    }

    // Creating second child (only from the parent process)
    pid_t id2 = {0};
    if (id1 != 0)
    {
        id2 = fork();
        if (id2 == -1)
        {
            FATAL_ERR("An error occured with the second fork", 2);
        }
    }

    // Set the start and end of the share of the array for each child
    int start = {0};
    int end = {0};
    if (id1 == 0)
    {
        start = 0;
        end = arr_size / 2;
    }
    else if (id2 == 0)
    {
        start = arr_size / 2;
        end = arr_size;
    }

    // Compute the partial sum in each child process
    int par_sum = {0};
    if (id1 == 0 || id2 == 0)
    {
        for (int i = start; i < end; i++)
        {
            par_sum += arr[i];
        }
    }

    // Print partial results
    if (id1 == 0)
    {
        printf("[CHILD 1] computed partial sum: %d.\n", par_sum);
    }
    else if (id2 == 0)
    {
        printf("[CHILD 2] computed partial sum: %d.\n", par_sum);
    }

    // Send the result from the first child to parent
    if (id1 == 0)
    {
        // We have no business with the second pipe
        close(0 [pfds2]);
        close(1 [pfds2]);

        // No read operation in the child process
        close(pfds1[0]);

        ssize_t writeerr = write(pfds1[1], &par_sum, sizeof(par_sum));
        if (writeerr == -1)
        {
            close(pfds1[1]);
            FATAL_ERR("An error occured with write in the child process.", 3);
        }

        close(pfds1[1]);
    }
    // Send the result from the second child to parent
    else if (id2 == 0)
    {
        // We have no business with the first pipe
        close(0 [pfds1]);
        close(1 [pfds1]);

        close(pfds2[0]);

        ssize_t writeerr = write(pfds2[1], &par_sum, sizeof(par_sum));
        if (writeerr == -1)
        {
            close(pfds2[1]);
            FATAL_ERR("An error occured with write in the first child process.", 3);
        }

        close(pfds1[1]);
    }
    // Receive the results in the parent processes and compute the total sum
    else
    {
        // No write operation
        close(pfds1[1]);
        close(pfds2[1]);

        // Get the sum from the first child process (read from the first pipe)
        int sum_from_1st_child = {0};
        ssize_t readerr = read(pfds1[0], &sum_from_1st_child, sizeof(sum_from_1st_child));
        if (readerr == -1)
        {
            close(pfds1[0]);
            FATAL_ERR("An error occured with write in the first child process.", 4);
        }
        close(pfds1[0]);

        // Get the sum from the second child process (read from the second pipe)
        int sum_from_2nd_child = {0};
        readerr = read(pfds2[0], &sum_from_2nd_child, sizeof(sum_from_2nd_child));
        if (readerr == -1)
        {
            close(pfds2[0]);
            FATAL_ERR("An error occured with write in the second child process.", 4);
        }
        close(pfds2[0]);

        // Compute the total sum
        int total_sum = par_sum + sum_from_1st_child + sum_from_2nd_child;
        printf("Total sum: %d.\n", total_sum);

        // wait for two child processes
        wait(NULL);
        wait(NULL);
    }

    return 0;
}
