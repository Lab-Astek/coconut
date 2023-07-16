/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test L5
*/

#include "pointers.h"

void *my_ok_func(char const *str)
{
    void const *ptr = str;
    void *ptr2 = (void *) ptr;

    return ptr2;
}

void*missing_space(char *str)
{
    char*str2 = str;

    return str2;
}

void* attached_wrongly(char *str)
{
    char* str2 = str;

    return str;
}

void * extra_space(char *str)
{
    char * str2 = str;

    return str;
}

int main(int argc, char **argv)
{
    char** wrong = argv;
    char const * const ok2 = wrong[0];
    char const *const ok3 = ok2;

    return ok3[0];
}
