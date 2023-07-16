/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test L5
*/

#include <stdbool.h>

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

int ok_blank(void)
{
    int i = 0;

    return i;
}

bool ok_no_vars(void)
{
    return true;
}

int main(void)
{
    return blank_in_the_mid() + missing_blank() + ok_blank() + ok_no_vars();

}
