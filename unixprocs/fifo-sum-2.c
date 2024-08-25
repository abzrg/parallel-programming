// send data from fifo-sum-1 -> fifo-sum-2, using a fifo and compute the sum

#include <stdio.h>
#include <unistd.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
    int arr[5] = {0};

    int fd = open("sum", O_RDONLY);
    if (fd == -1)
    {
        return 1;
    }

    // Receive the data from fifo-sum-1 process
    for (int i = 0; i < 5; i++)
    {
        int rerr = read(fd, &arr[i], sizeof(*arr));
        if (rerr == -1)
        {
            return 2;
        }

        printf("Received %d\n", arr[i]);
    }
    // Close the fifo (for reading)
    close(fd);

    // Compute the sum
    int sum = {0};
    for (int i = 0; i < 5; i++)
    {
        sum += arr[i];
    }
    printf("Sum = %d\n", sum);

    return 0;
}
