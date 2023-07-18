/*
** EPITECH PROJECT, 2023
** f4
** File description:
** f4
*/

#include <stdlib.h>

void *make_calls(void)
{
    size_t size = sizeof(int);
    void *ptr = malloc(size);

    ptr = realloc(ptr, size);
    ptr = realloc(ptr,size);
    ptr = realloc(ptr , size);
    ptr = realloc(ptr ,size);
    ptr = realloc(ptr, size );
    ptr = realloc( ptr, size);
    ptr = realloc (ptr, size);
    ptr = realloc ( ptr, size);
    return ptr;
}

int main(void)
{
    free(make_calls());
}
