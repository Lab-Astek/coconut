/*
** EPITECH PROJECT, 2023
** coconut tests
** File description:
** test
*/

#include <stdbool.h>

int make_calculations(int a, bool b, int c)
{
    c = -1;
    c = - 1;
    c = -a;
    c = - a;
    c = !c;
    c = ! c;
    c = ~a;
    c = ~ c;
    return (a + b + c);
}

int increments(int a)
{
    a++;
    a ++;
    ++a;
    ++ a;
    a--;
    a --;
    --a;
    -- a;
    return a;
}

int main(void)
{
    return make_calculations(1, 2, 3);
}
