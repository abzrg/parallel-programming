/*
   Named pipes (fifo) as a replacement for pipes for communication between processes that are not in
   the same heirarchies.

   When openfing a fifo to write to it (O_WRONLY) blocks the process and waits for another
   process/thrread to read the fifo, and when that happens the process continue after the call to
   open()

   Similarly when opening a fifo for reading it (O_RDONLY) blocks and waits for another
   process/thread to write to the fifo.

   If a files is opened for both reading and writing the call to open will execute immediately as
   fifo is accessed for both reading and writting.

   It throws an error and errno (EEXIST) when the fifo is already created.

   current shell                                           another shell
   -------------                                           =============
   $ ./fifo                                                $ cat myfifo

*/

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    int fifoerr = mkfifo("myfifo", 0777);
    if (fifoerr == -1)
    {
        if (errno != EEXIST)
        {
            fprintf(stderr, "Could not create file.");
            return 2;
        }
    }

    int fd = open("myfifo", O_WRONLY);

    int x = 'a';
    int werr = write(fd, &x, sizeof(x));
    if (werr == -1)
    {
        return 2;
    }

    close(fd);

    return EXIT_SUCCESS;
}
