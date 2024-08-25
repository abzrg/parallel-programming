/*

athr: Ali Bozorgzadeh
file: 04_pi_omp_pad.c
date: Apr 5 2024
desc: Calculates pi by integrating f function using OpenMP.
      In this version, a pdding is used to avoid false sharing.
      This version may suffer from false sharing in another platform depending
      on the size of the cache line.
*/

#include <stdio.h>
#include <omp.h>

// Assuming 64 byte (8 * sizeof(double)) L1 cache size
//   Apparently there aren't any portable way to determine
//   the cache line size in C
#define PAD_LEN 8

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
    double sums[NUM_THREADS][PAD_LEN] = { 0 };

    double eval_point = { 0.5 }; // 0.5: middle, 0: left, 1: right

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int id = omp_get_thread_num();

        int num_threads_local = omp_get_num_threads();
        if (id == 0) {
            num_threads = num_threads_local;
        }

        for (int i = id; i < (int)num_intervals; i += num_threads_local) {
            double x = dx * (i + eval_point);
            sums[id][0] += f(x) * dx;
        }
    }

    double pi = { 0 };
    for (int i = 0; i < num_threads; ++i) {
        pi += sums[i][0];
    }

    printf("pi = %lf\n", pi);

    return 0;
}
