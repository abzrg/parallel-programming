#include <pthread.h> // POSIX Threads; add this option to gcc when compiling: -pthread
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// A function that pthreads_create calls/executes upon creation of the thread
void *routine(void *routine_arg)
{
    (void) routine_arg;
    sleep(3);
    return NULL;
}

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    // Define threads
    const size_t num_threads = {4};
    pthread_t threads[4] = {0};

    // Initialize and start threads
    for (size_t i = 0; i < num_threads; i++)
    {
        printf("Starting thread %zu / %zu\n", i, num_threads);

        // NULL: default settings
        int terr = pthread_create(&threads[i], NULL, &routine, NULL);

        // Check for thread initialization errors
        if (terr != 0)
        {
            fprintf(stderr, "Failed initializing the thread.");
            exit(1);
        }
    }

    // Wait for threads to finish their execution (join the main thread)
    printf("Joining main thread...\n");
    for (size_t i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
        printf("Joined thread %zu / %zu\n", i, num_threads);
    }

    return 0;
}
