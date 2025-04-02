# Sum Reduction


- `MPI_Scatterv()` is a more flexible version of `MPI_Scatter()`.
  It allows for variable-sized chunks to be sent to each process instead of forcing all processes to receive the same amount of data.

    ```c
    int MPI_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, 
                   MPI_Datatype sendtype, void *recvbuf, int recvcount, 
                   MPI_Datatype recvtype, int root, MPI_Comm comm);
    ```

    - `sendbuf`: The buffer in the root process containing all data.
    - `sendcounts`: An array specifying how many elements each process gets.
    - `displs`: An array specifying the starting index in sendbuf for each process.
    - `sendtype`: The data type of elements in sendbuf.
    - `recvbuf`: Buffer where each process receives its chunk.
    - `recvcount`: Number of elements received (should match sendcounts[rank]).
    - `recvtype`: Data type of elements in recvbuf.
    - `root`: The rank of the process that sends the data.
    - `comm`: The communicator (typically MPI_COMM_WORLD).

This is useful when:
- The total data size is not evenly divisible by the number of processes.
- Different processes need different amounts of data.

Alternative code using `MPI_Scatterv()`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0
#define ARRAY_SIZE 100

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *send_buf = NULL, *sendcounts = NULL, *displs = NULL;
    int chunk_size = ARRAY_SIZE / size;
    int remainder = ARRAY_SIZE % size;

    if (rank == MASTER) {
        send_buf = malloc(sizeof(int) * ARRAY_SIZE);
        for (int i = 0; i < ARRAY_SIZE; ++i)
            send_buf[i] = rand() % 100;

        sendcounts = malloc(sizeof(int) * size);
        displs = malloc(sizeof(int) * size);

        int offset = 0;
        for (int i = 0; i < size; i++) {
            sendcounts[i] = chunk_size + (i < remainder ? 1 : 0);  // Distribute remainder elements
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    // Allocate appropriate space for each rank
    int recv_count = chunk_size + (rank < remainder ? 1 : 0);
    int *recv_buf = malloc(sizeof(int) * recv_count);

    // Scatter data
    MPI_Scatterv(send_buf, sendcounts, displs, MPI_INT,
                 recv_buf, recv_count, MPI_INT,
                 MASTER, MPI_COMM_WORLD);

    // Local sum
    int local_sum = 0;
    for (int i = 0; i < recv_count; i++) {
        local_sum += recv_buf[i];
    }

    // Reduce sum
    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);

    if (rank == MASTER) {
        printf("Total sum: %d\n", global_sum);
        free(send_buf);
        free(sendcounts);
        free(displs);
    }

    free(recv_buf);
    MPI_Finalize();
    return 0;
}
```
