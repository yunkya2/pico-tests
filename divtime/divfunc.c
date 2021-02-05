#include "divfunc.h"

int __real___aeabi_idiv(int, int);

int idiv_hard(int a, int b)
{
    return a / b;
}

int idiv_soft(int a, int b)
{
    return __real___aeabi_idiv(a, b);
}

int idiv_dummy(int a, int b)
{
    return 0;
}
