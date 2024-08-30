/*

file: printing_primes.c
date: Aug 30th 2024
desc:

    PASS ARGUMENTS TO THREADS

    This is done via `phread_create`, and passing pointers to arguments
    The problem with this implementation is management of resources
    Two different stacks (main and print_primes) manage one resource (void *arg).

*/

#include "helper.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10UL

const int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void *print_primes(void *arg)
{
    size_t idx = *(size_t *) arg;
    printf("%d ", primes[idx]);
    free(arg);
    return NULL;
}

int main(void)
{
    pthread_t ths[NUM_THREADS];
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        // This is needed as the value of i changes constantly. Any delay between the creation and
        // use of i in the thread routine means that the routine is using the wrong value of i.
        // Therefore, in each iteration and during the creation of each thread, we store it in
        // another unique variable that holds the value specific to each thread.
        int *unique_idx = malloc(sizeof(*unique_idx));
        *unique_idx = i;
        CREATE_THREAD(&ths[i], NULL, &print_primes, unique_idx);
        // However we also need to free it when thread is done, and this is happening inside the
        // thread routine itself, and not here.
    }

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        JOIN_THREAD(ths[i], NULL);
    }

    printf("\n");

    return 0;
}
