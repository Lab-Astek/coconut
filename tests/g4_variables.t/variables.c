/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test g4
*/

#include "variables.h"

int my_extern_var = 3;

int main(void)
{
    static int static_var_ok = 0;
    my_global_var = -1;
    my_static_var = 3;
    my_tentative_var = 8;
    my_extern_var = 84;
    return my_global_var;
}
