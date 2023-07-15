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

void test_if(int a)
{
    if (a > 0) {
        printf("a is positive\n");
    } else if (a < 0) {
        printf("a is negative\n");
    } else {
        printf("a is zero\n");
    }
    if (a > 0)
        printf("a is valid\n");
    else return;
    a--;
    if (a > 0)
        printf("a is still valid\n"); else
        return;
}

void test_loops(void)
{
    unsigned j = 10;

    for (int i = 0; i < 10; i++) printf("i is %d\n", i);
    while (j > 5) j--;
    for (int i = 0; i < 3; i += 1)
        printf("i is %d\n", i);
    while (j > 2)
        j--;
    switch (j) {
    case 0:
    case 1: break;
    case 2:
        printf("j is 2\n");
        break;
    }
}

void test_do_while(void)
{
    int j = 0;

    do {
        printf("Enter a character: ");
        j = getchar();
    } while (j > 32);
    do { printf("Enter a character: ");
        j = getchar();
    } while (j > 32);
    do {
        printf("Enter a character: ");
        j = getchar(); } while (j > 32);
}

int main(void)
{
    int b = 0;

    switch (b) {
    case 0: printf("0\n"); break;
    case 1:
        b = 3;
    case 7:
        if (getchar() > 64) {
            printf("yes\n");
            break;
        } else
            return 0;
    }
    free(allocate_stuff());
}
