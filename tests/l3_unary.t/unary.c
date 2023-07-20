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

bool sizeof_is_an_unary_operator(void)
{
    unsigned long a = sizeof(int);

    a += sizeof (long);
    a += sizeof (a);
    a += sizeof(a);
    a += sizeof a;
    a += _Alignof (long);
    a += _Alignof(int);
    return a;
}

int main(void)
{
    return make_calculations(1, 2, 3);
}
