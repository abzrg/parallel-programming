/*
file: race_cond_1.c
date: Aug 30th 2024
desc:

    Solving the race condition using the mutex.
    A mutex (mutual exclusion) is sort of a lock around a piece of code.
    Here's the pseudo code that shows how a mutex conseptually does its job:

        [global] int lock = 0;

        ...

        fn routine():
            if lock == 1:
                // wait until the lock is 0
            lock = 1
            // rest of the code

*/

#include "helper.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int mails = {0};
pthread_mutex_t mutex = {0}; // Must be initialized properly later when threads are declared

void *routine(void *routine_arg)
{
    (void) routine_arg;
    for (size_t i = 0; i < 1000; i++)
    {
        // Surround the code that is causing the race condition with mutex gaurd (lock & unlock)
        pthread_mutex_lock(&mutex);
        mails++; // Line 6 -> L6
        pthread_mutex_unlock(&mutex);

        // !Note that although this solves the problem but make the program run slower. That's
        // because of the overhead mutex brings into the picture (lock and unlock).
    }

    return NULL;
}

int main(void)
{
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);

    int terr = {0};
    terr = pthread_create(&t1, NULL, &routine, NULL);
    if (terr != 0)
        fatal_log(1, "Failed initializing the the thread p1.\n");
    terr = pthread_create(&t2, NULL, &routine, NULL);
    if (terr != 0)
        fatal_log(2, "Failed initializing the the thread p2.\n");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Free resources allocated for a mutex
    pthread_mutex_destroy(&mutex);

    printf("%d\n", mails);

    return 0;
}
