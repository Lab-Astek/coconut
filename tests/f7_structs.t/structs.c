/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test rule F7 - structs passed by value
*/

#include "structs.h"

void move_val(pos_t pos)
{
    pos.x += 1;
    pos.y += 1;
}

void move_ptr(pos_t *pos)
{
    pos->x += 1;
    pos->y += 1;
}

void move_kw_val(struct position pos)
{
    pos.x += 1;
    pos.y += 1;
}

void move_kw_ptr(struct position *pos)
{
    pos->x += 1;
    pos->y += 1;
}

int main(void)
{
    return 0;
}
