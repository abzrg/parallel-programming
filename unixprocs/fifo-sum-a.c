// send and receive back and forth between the fifo-sum-a and fifo-sum-b.
// 1. write the data from fifo-sum-a to fifo
// 2. receive the data in fifo-sum-b from fifo
// 3. compute the sum in fifo-sum-b
// 4. write the result back to fifo-sum-a to write on the screen.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

int main(void)
{
    int arr[5] = {0};

    srand(time(NULL));
    for (int i = 0; i < 5; i++)
    {
        arr[i] = rand() % 100;
        printf("Generated %d\n", arr[i]);
    }

    const char *fifo_path = "sum";
    int fd = open(fifo_path, O_WRONLY);
    if (fd == -1)
    {
        return 1;
    }

    int werr = write(fd, arr, 5 * sizeof(*arr));
    if (werr == -1)
    {
        return 2;
    }
    printf("Write data to fifo \"%s\".\n", fifo_path);

    close(fd);

    fd = open(fifo_path, O_RDONLY);
    if (fd == -1)
    {
        return 3;
    }

    int sum = {0};
    int rerr = read(fd, &sum, sizeof(sum));
    if (rerr == -1)
    {
        return 4;
    }

    printf("Received sum from \"%s\" = %d\n", fifo_path, sum);

    close(fd);

    return EXIT_SUCCESS;
}
