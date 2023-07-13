/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test g4
*/

#include "ternaries.h"
#include <stdio.h>

int chained_ternaries(int a)
{
    return a == 0 ? 0 : a == 1 ? 1 : 2;
}

int chained_ternaries_paren(int a)
{
    return a == 0 ? 0 : (a == 1 ? 1 : 2);
}

int nested_ternaries(int a)
{
    return a ? (a == 1 ? 1 : 2) : 0;
}

int ternary_as_control_flow(int a)
{
    a < 0 ? nested_ternaries(a) : 0;
    return 0;
}

int main(void)
{
    double d = (getchar() == 'a' ? 1.0 : 2.0);

    ternary_as_control_flow(0);
    printf(FP_FMT(d), d);
    putchar('\n');
    if (d == 1.0 ? 1 : 0)
        3 ? nested_ternaries(1) : nested_ternaries(2);
    else
        1 ? nested_ternaries(1) : nested_ternaries(2);
}
