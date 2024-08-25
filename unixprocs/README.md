# Unix Processes

## Order of source code to read

- `hello.c`: saying hello in multiple processes
- `wait.c`: waiting for child processes to prevent them from being zombies!
- `getpid.c`: get the id of processes to identify each process
- `forkfork.c`: calling fork() multiple times
- `pipe.c`: communicate between two processes
- `sum-1.c`: sum of array done by parent and child proc
- `sum-2.c`: sum of array done by two child (results are sent back to parent)
- `fifo.c`: demo of how FIFO works
- `fifo-sum-1.c`, `fifo-sum-2.c`: demo on sending data between two arbitrary process (run each in one
  terminal)
  - the fifo file must be created using `mkfifo sum`
- `fifo-sum-a.c`, `fifo-sum-b.c`: send data back and forth (multiple open)
  - the fifo file must be created using `mkfifo sum`

## Learning resources

- [Fork() system call tutorial](https://www.youtube.com/watch?v=xVSPv-9x3gk)
- [fd, dup(), dup2() system call tutorial](https://www.youtube.com/watch?v=EqndHT606Tw)
- [Pipe() tutorial for linux](https://www.youtube.com/watch?v=uHH7nHkgZ4w)
- [Understanding the Pipe System Call (popen3 example in C)](https://youtu.be/8AXEHrQTf3I)

## Header files

```c
#include <stdlib.h> // read, write
#include <unistd.h>  // fork(), pipe()
#include <sys/wait.h> // wait()

#include <sys/types.h>
#include <sys/stat.h> // mkfifo()

#include <fcntl.h> // open()
```

## Questions

- Why can't pipe's be shared by processes that are not in the same hierarchies?
> Maybe because there's no mechanism to transfer the same file descriptor (copy two integers) to those processes?
> therefore instead of pipes fifo's are used. they are similar to files and they are also known as named pipes
