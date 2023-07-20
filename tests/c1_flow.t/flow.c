/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test C1
*/

#include <stdlib.h>

int nested_if(void)
{
    if (1) {
        if (1 && 1) {
            if (!0) {
                return 0;
            }
        }
    }
}

void braceless_if_else(void)
{
    if (1)
        nested_if();
    else
        if (2)
            nested_if();
        else
            if (3)
                nested_if();
}

int calc_ok(int i)
{
    if (1)
        return 0;
    else {
        while (0)
            i = i + 1;
        if (1 == 2 / 2)
            return 1;
    }
}

static int fat_function(char *s)
{
    int f = fat_function(s);
    int z = 3 * (f);
    char *b = malloc(10);

    if (b == NULL) {
        return (84);
    } else if (f == -1) {
        return (84);
    } else if (z == 84) {
        return (84);
    }
    return (0);
}

int main(void)
{
}
