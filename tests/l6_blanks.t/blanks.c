/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test L5
*/

#include "blanks.h"

int blank_in_the_mid(void)
{
    int ok = 0;

    if (ok == 0)
        return ok;

    return -1;
}

int missing_blank(void)
{
    int i = 0;
    return i;
}

long ok_with_macros(char *buffer)
{
    long description_size = MIN(*((long *) buffer),
        MAX_LENGTH);

    return description_size;
}

int ok_no_vars(void)
{
    return 1;
}

int main(void)
{
    return blank_in_the_mid() + missing_blank() + ok_no_vars();

}
