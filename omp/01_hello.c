#include <stdio.h>

#include <omp.h>

int main(void)
{
    // Parallel region with default number of threads
    #pragma omp parallel
    {


        // Runtime library function to return a thread ID
        // To change default: export OMP_NUM_THREADS=<num_threads>
        int ID = omp_get_thread_num();
        printf(" hello(%d)", ID);
        printf(" world(%d)\n", ID);


    } // end of the parallel region

    return 0;
}
