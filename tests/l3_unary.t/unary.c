/*
** EPITECH PROJECT, 2023
** coconut tests
** File description:
** test
*/

#include <stdbool.h>
#include <iso646.h>

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

bool macro_operators(bool a, bool b)
{
    return (not a and b) or (compl (a << 3) or b);
}

int main(void)
{
    return make_calculations(1, 2, 3);
}
