/*

athr: Ali Bozorgzadeh
file: 05_pi_omp_sync.c
date: Apr 12 2024
desc: Calculates pi by integrating f function using OpenMP.
      In this version, a critical synchronization has been used.
      This version does not suffer from false sharing.
*/


#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 8
static int num_threads = { 0 };

double f(double x)
{
    return 4.0 / (1 + x * x);
}

static const double domain_len = { 1.0 };
static const size_t num_intervals = { 1000000000 };
static const double dx = { domain_len / num_intervals };

int main(void)
{
    double pi = { 0 };

    double eval_point = { 0.5 }; // 0.5: middle, 0: left, 1: right

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int id = omp_get_thread_num();

        int num_threads_local = omp_get_num_threads();
        if (id == 0) {
            num_threads = num_threads_local;
        }

        // Similar to previous code, create a scalar local each thread to
        // accumulate partial sums.
        double local_sum = { 0 };
        for (int i = id; i < (int)num_intervals; i += num_threads_local) {
            double x = dx * (i + eval_point);
            local_sum += f(x) * dx;
        }

        // Again local_sum goes "out of scope" beyond the parallel region and
        // therefore must be used.
        // Critical region protects summation into pi so that updates don't
        // conflict.
        #pragma omp critical
            pi += local_sum; // Unlike atomic, more elaborate operations can be
                              // done here.
    }

    printf("pi = %lf\n", pi);

    return 0;
}
