#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
    int a, b, q, r;

    stdio_init_all();
    printf("Start divider test\n");

    a = 12345678;
    b = 1234;

    *(int *)0xd0000060 = a;
    *(int *)0xd0000064 = b;

    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");

    r = *(int *)0xd0000074;
    q = *(int *)0xd0000070;

    printf("%u / %u = %u mod %u\n", a, b, q, r);

    while (1)
        ;
}
