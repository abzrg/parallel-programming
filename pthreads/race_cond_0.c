/*
file: race_cond_0.c
date: Aug 30th 2024
desc:

    Three steps (machine code / assembly) of incrementing a (in this case global) variable:
    1. read (the value of the variable in memory into a register in CPU)
    2. increment (the value in register and not in memory)
    3. write (overwrite the value sitting at the memory from which we read the value)

    These operations may be intertwined depending how the operating systems schedules different
    threads. Therefore there must be a mechanism that, when a threads starts these three steps,
    prevents other threads from executing the same sequence of machine code, i.e. locks them.

    Note: race condition only happens on systems with multiple cores on a processor.

*/

#include "helper.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int mails = {0};

void *routine(void *routine_arg)
{
    (void) routine_arg;
    // Increase '1000' to higher values if you don't see incorrect results
    for (size_t i = 0; i < 1000; i++)
    {
        mails++; // Line 6 -> L6
    }

    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    int terr = {0};
    terr = pthread_create(&t1, NULL, &routine, NULL);
    if (terr != 0)
        fatal_log(1, "Failed initializing the the thread p1.\n");
    terr = pthread_create(&t2, NULL, &routine, NULL);
    if (terr != 0)
        fatal_log(2, "Failed initializing the the thread p2.\n");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("%d\n", mails);

    return 0;
}
