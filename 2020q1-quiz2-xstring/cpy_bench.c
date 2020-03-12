#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "xs.h"

#define CPYS cpys
int cpys;

typedef enum { XSTR, CSTR } MODE;

static inline double micro_time()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}

double test_x_str(xs *x_str, xs x_in)
{
    double x_start, x_finish;
    x_start = micro_time();
    for (int i = 0; i < CPYS; i++)
        xs_cpy(&x_str[i], &x_in);
    x_finish = micro_time();

    return x_finish - x_start;
}

double test_c_str(char **c_str, char *c_in)
{
    double c_start, c_finish;
    c_start = micro_time();
    for (int i = 0; i < CPYS; i++)
        strcpy(c_str[i], c_in);
    c_finish = micro_time();

    return c_finish - c_start;
}

int main(int argc, char **argv)
{
    if (argc != 4)
        return 1;
    MODE mode = atoi(argv[3]);
    cpys = atoi(argv[2]);
    int len = strlen(argv[1]);
    double time = 0;

    if (mode == XSTR) {
        xs x_in;
        xs_new(&x_in, argv[1]);
        xs x_str[CPYS];
        for (int i = 0; i < CPYS; i++)
            xs_new(&x_str[i], "");
        time = test_x_str(x_str, x_in);
#ifndef PLOT
        printf("MODE: XSTR\n");
        printf("refcnt: %ld\n", xs_refcnt(&x_in));
#endif
    } else {
        char *c_in = argv[1];
        char *c_str[CPYS];
        for (int i = 0; i < CPYS; i++)
            c_str[i] = malloc((len + 1) * sizeof(char));
        time = test_c_str(c_str, c_in);
#ifndef PLOT
        printf("MODE: CSTR\n");
#endif
    }


    printf("time: %lf\n", time);
    return 0;
}
