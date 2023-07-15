/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test L5
*/

#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>

void *allocate_stuff(void)
{
    int a;
    int b;
    int c;
    void *ptr;

    a = b = c = 0;
    ptr = NULL;
    a++; b++;
    if ((ptr = malloc(10)) == NULL)
        return NULL;
    c++;
    if (a > b) return ptr;
    free(ptr);
    return NULL;
}

int main(void)
{
    free(allocate_stuff());
}
