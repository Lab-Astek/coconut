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

int other_paren(void)
{
    int a = 0;

    a = (a);
    a = ( a);
    a = (a );
    a = ( a );
    a = (a) + (a);
    a = (a) + ( a);
    return (a);
}

int semi_stuff(void)
{
    int a = 0;

    a = (a) ;
    a = 3 + 7 ;
    return (a);
}

int main(void)
{
    free(make_calls());
}
