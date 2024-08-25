// send data from fifo-sum-1 -> fifo-sum-2, using a fifo

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

    // Fill the array with random number \in [0, 100)
    srand(time(NULL));
    for (int i = 0; i < 5; i++)
    {
        arr[i] = rand() % 100;
    }

    // Open an fifo called "sum" to wrie the sum into it
    int fd = open("sum", O_WRONLY);
    if (fd == -1)
    {
        return -1;
    }

    // Write the sum into the fifo
    for (int i = 0; i < 5; i++)
    {
        int werr = write(fd, &arr[i], sizeof(*arr));
        if (werr == -1)
        {
            return 2;
        }

        printf("Wrote %d\n", arr[i]);
    }
    // Close the fifo (for writing)
    close(fd);

    return 0;
}
