/*

file: array_sum.c
ahtr: Ali Bozorgzadeh
date: Aug 30th 2024
Lcns: MIT
desc:
    Sum of arrays using multiple threads.

    Structures are used to path the data around. To store the sum dynamic memory for one integer is
    allocated, passed to thread, overwritten with the actual value of the partial sum, and returned
    to the main thread, from where it is freed.
*/

#include "helper.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5UL

const int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
#define NUM_PRIMES ARR_LEN(primes)

int global_sum = {0};

typedef struct
{
    size_t begin;
    size_t end; // exclusive
} Range;

typedef struct
{
    Range r;
    void *sum_ptr;
} Arg;

void *sum_array(void *argument)
{
    Arg arg = *(Arg *) argument;
    size_t begin = arg.r.begin;
    size_t end = arg.r.end;

    int sum = {0};

    for (size_t j = begin; j < end; j++)
    {
        sum += primes[j];
    }

    *((int *) arg.sum_ptr) = sum;

    return arg.sum_ptr;
}

int main(void)
{
    pthread_t th[NUM_THREADS];
    Arg args[NUM_THREADS];

    //- Distribute/project the array to threads
    // size of the chunk of array data passed to threads for the calculation of the partial sum
    size_t range_size = NUM_PRIMES / NUM_THREADS;
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        size_t begin = i * range_size;
        // Make sure the last thread gets the remaining elements
        size_t end = (i == NUM_THREADS - 1) ? NUM_PRIMES : begin + range_size;
        Range r = {.begin = begin, .end = end};

        int *sum_ptr = malloc(sizeof(*sum_ptr));
        if (sum_ptr == NULL)
        {
            fatal_log(-1, "Failed to allocate memory");
        }

        args[i] = (Arg){.r = r, .sum_ptr = sum_ptr};
        CREATE_THREAD(&th[i], NULL, &sum_array, &args[i]);
    }

    int reduced_sum = {0};

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        int *sum = NULL;
        JOIN_THREAD(th[i], (void **) &sum);
        reduced_sum += *sum;
        free(sum);
    }

    printf("Sum = %d\n", reduced_sum);

    return 0;
}
