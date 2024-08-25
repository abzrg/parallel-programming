/*

athr: Ali Bozorgzadeh
file: 02_pi_serial.c
date: Apr 5 2024
desc: Calculates pi by integrating f function

*/

#include <omp.h>
#include <stdio.h>

double f(double x)
{
    return 4.0 / (1 + x * x);
}

static const long double domain_len = { 1.0 };
static const size_t num_intervals = { 10000 };
static const long double dx = { domain_len / num_intervals };

int main(void)
{
    long double pi = { 0 };
    // Where in the interval the function should be evaluated
    double eval_point = { 0.5 }; // 0.5: middle, 0: left, 1: right
    for (size_t i = 0; i < num_intervals; ++i) {
        long double x = dx * (i + eval_point);
        pi += f(x) * dx;
    }

    printf("pi = %Lf\n", pi);

    return 0;
}
