/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test L5
*/

#include <stddef.h>

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

char *ok_blank(void)
{
    char *i = NULL;

    return i;
}

int ok_no_vars(void)
{
    return 1;
}

int main(void)
{
    return blank_in_the_mid() + missing_blank() + ok_no_vars();

}
