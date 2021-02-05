#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "divfunc.h"

int64_t measure_div_time(int count, int (*divfunc)(int, int))
{
    absolute_time_t t1, t2;
    int i;

    t1 = get_absolute_time();
    for (i = 0; i < count; i++) {
        divfunc(12345678, 1234);
    }
    t2 = get_absolute_time();
    return absolute_time_diff_us(t1, t2);
}

int main()
{
    uint64_t th, ts, td;

    stdio_init_all();
    printf("Start divider time test\n");

    th = measure_div_time(1000000, idiv_hard);
    ts = measure_div_time(1000000, idiv_soft);
    td = measure_div_time(1000000, idiv_dummy);
    printf("count: hard=%llu soft=%llu\n", th - td, ts - td);

    while (1)
        ;
}
