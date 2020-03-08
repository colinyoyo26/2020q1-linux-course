#include "xs.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define CPYS cpys
int cpys;

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
    if (argc != 3)
        return 1;
    cpys = atoi(argv[2]);
    int len = strlen(argv[1]);
    xs x_in;
    char *c_in = argv[1];
    xs_new(&x_in, argv[1]);
    assert(!strcmp(xs_data(&x_in), c_in));

    xs x_str[CPYS];
    char *c_str[CPYS];

    for (int i = 0; i < CPYS; i++) {
        xs_new(&x_str[i], "");
        c_str[i] = malloc((len + 1) * sizeof(char));
    }

    double x_time = test_x_str(x_str, x_in), c_time = test_c_str(c_str, c_in);

    for (int i = 0; i < CPYS; i++){
        assert(!strcmp(xs_data(&x_str[i]), c_in));
        assert(!strcmp(c_str[i], c_in));
    }

    printf("refcnt: %ld\n", xs_refcnt(&x_in));
    printf("x_time: %lf \nc_time: %lf\n", x_time, c_time);
    return 0;
}