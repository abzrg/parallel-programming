/*

file: roll_dice.c
date: Aug 30th 2024
desc:

    GET RETURN VALUE

    To return the value from a thread we return the address of the variable that holds the variable.
    It's the `pthread_join`'s second argument that handles the value to us.
    Therefore we have to create a variable that will hold the value "returned" by thread routine,
    and then pass to the thread routine via the call to `pthread_create` and finally receive the
    result by once again passing the address of the variable to the `pthread_join`.
    Alternatively, we could malloc-ed memory to hold the return value and then free it in the caller
    function.

*/

#include "helper.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void *roll_dice(void *subr_arg)
{
    (void) subr_arg;

    int *val = malloc(sizeof(*val));
    *val = (rand() % 6) + 1;

    return val;
}

int main(void)
{
    // Seed the random number generator with the current time
    srand(time(NULL));

    pthread_t th = {0};
    CREATE_THREAD(&th, NULL, &roll_dice, NULL); // safely creates a thread (checks for erros)

    int *res = NULL;
    JOIN_THREAD(th, (void **) &res); // safely joins to a thread (checks for erros)
    printf("Result = %d\n", *res);
    // `pthread_join` modifies where res is pointing at. Now res pointing to a resource `roll_dice`
    // has allocated, thus we can release it.
    free(res);

NOTE: /* But this is not good practice. That's because we are freeing a resource that is not
         allocated in the current stack frame. We might forget to free it if we don't immediately
         observe it being allocated. */

    return 0;
}
