#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mpi.h>

#define MASTER 0
#define ARRAY_SIZE 100

int main(int argc, char **argv)
{
    srand(0);

    MPI_Init(&argc, &argv);

    int size = 0, rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *send_buf = NULL;
    if (rank == MASTER)
    {
        send_buf = malloc(sizeof(*send_buf) * ARRAY_SIZE); // <- FREE

        for (size_t i = 0; i < ARRAY_SIZE; ++i)
            send_buf[i] = 1; // (rand() % ARRAY_SIZE);
    }

    // If size is not a divisor of ARRAY_SIZE, this will result in data loss or
    // segmentation faults.
    // Fix: Use MPI_Scatterv() instead of MPI_Scatter(), or ensure that size
    // divides ARRAY_SIZE perfectly.
    size_t chunk_size = 100 / size;
    int *recv_buf = malloc(sizeof(*recv_buf) * chunk_size);
    // Distribute data from one task to all tasks in a group. Equivalent to n
    // send operations in a for loop:
    //     MPI_Send(send_buf + i * sendcount * extent(sendtype), sendcount,
    //              sendtype, i, ...);
    // The `rank == MASTER ? send_buf : NULL` is done to prevent undefined
    // behavior.
    MPI_Scatter(rank == MASTER ? send_buf : NULL, chunk_size, MPI_INT, recv_buf,
                chunk_size, MPI_INT, MASTER, MPI_COMM_WORLD);

    // Local calculation
    int local_sum = 0;
    for (size_t i = 0; i < chunk_size; ++i)
    {
        local_sum += recv_buf[i];
    }

    // Take 1 value from each processor and sum them into one value
    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, MASTER,
               MPI_COMM_WORLD);

    if (rank == MASTER)
        printf("Total: %d\n", global_sum);

    if (rank == MASTER)
        free(send_buf);
    free(recv_buf);

    MPI_Finalize();

    return 0;
}
