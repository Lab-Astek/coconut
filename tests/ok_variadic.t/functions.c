/*
** EPITECH PROJECT, 2023
** h1
** File description:
** h1
*/

#include <stdarg.h>

int make_all(int nb, ...)
{
    va_list ap;

    va_start(ap, nb);
    for (int i = 0; i < nb; i++)
        va_arg(ap, int);
    va_end(ap);
    return (0);
}

int main(void)
{
    make_all(3, 1, 2, 3);
    return (0);
}
