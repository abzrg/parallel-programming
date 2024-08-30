#pragma once

// vim: ft=c

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARR_LEN(ARR) (sizeof((ARR)) / sizeof((ARR[0])))

#define MAX_MSG_SIZE 256UL
extern inline void fatal_log(int err, const char *msg)
{
    if (err < 0)
    {
        perror(msg);
        exit(errno);
    }
    else
    {
        fprintf(stderr, "[%d] %s", err, msg);
        exit(err);
    }
}

#define CREATE_THREAD(THREAD_PTR, ATTR_PTR, ROUTINE_PTR, ARG_PTR)                                  \
    do                                                                                             \
    {                                                                                              \
        int terr = {0};                                                                            \
        terr = pthread_create((THREAD_PTR), (ATTR_PTR), (ROUTINE_PTR), (ARG_PTR));                 \
        if (terr != 0)                                                                             \
        {                                                                                          \
            char msg[MAX_MSG_SIZE] = {0};                                                          \
            snprintf(msg, MAX_MSG_SIZE, "Failed to create thread %s.\n", #THREAD_PTR);             \
            fatal_log(-1, msg);                                                                    \
        }                                                                                          \
    }                                                                                              \
    while (0)

#define JOIN_THREAD(THREAD, VALUE_POINTER)                                                         \
    do                                                                                             \
    {                                                                                              \
        int jerr = {0};                                                                            \
        jerr = pthread_join((THREAD), (VALUE_POINTER));                                            \
        if (jerr != 0)                                                                             \
        {                                                                                          \
            char msg[MAX_MSG_SIZE] = {0};                                                          \
            snprintf(msg, MAX_MSG_SIZE, "Failed to join thread %s.\n", #THREAD);                   \
            fatal_log(-1, msg);                                                                    \
        }                                                                                          \
    }                                                                                              \
    while (0)
