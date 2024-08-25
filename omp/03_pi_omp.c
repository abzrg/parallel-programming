/*

athr: Ali Bozorgzadeh
file: 03_pi_omp.c
date: Apr 5 2024
desc: Calculates pi by integrating f function using OpenMP

*/

#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 8  // This is a requested value
int num_threads = { 0 }; // This is the actuall number given by system

double f(double x)
{
    return 4.0 / (1 + x * x);
}

static const double domain_len = { 1.0 };
static const size_t num_intervals = { 10000 };
static const double dx = { domain_len / num_intervals };

int main(void)
{
    double sums[NUM_THREADS] = { 0 };
    // size_t chunk_size = num_intervals / NUM_THREADS;

    // Where in the interval the function should be evaluated
    double eval_point = { 0.5 }; // 0.5: middle, 0: left, 1: right

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int id = omp_get_thread_num();

        int num_threads_local = omp_get_num_threads();
        // Only one thread should write to global variables
        if (id == 0) {
            num_threads = num_threads_local;
        }

        /*
        int chunk_begin = (int)chunk_size * id;
        int chunk_end = (int)chunk_size * (id + 1);
        for (int i = chunk_begin; i < chunk_end; ++i) {
            double x = dx * (i + eval_point);
            sums[id] += f(x) * dx;
        }
        */

        double start_thread_time = omp_get_wtime();
        // Trick: using cyclic distrubtion of loop iterations
        for (int i = id; i < (int)num_intervals; i += num_threads_local) {
            double x = dx * (i + eval_point);
            sums[id] += f(x) * dx;
        }
        double end_thread_time = omp_get_wtime();

        printf("In thread %d, time taken: %f seconds\n", id,
               end_thread_time - start_thread_time);
    }

    // Calculate pi
    double pi = { 0 };
    for (int i = 0; i < num_threads; ++i) {
        pi += sums[i];
    }

    printf("\npi = %lf\n", pi);
    printf("%d\n", num_threads);

    return 0;
}
