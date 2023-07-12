/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test rule F7 - structs passed by value
*/

#pragma once

struct position {
    int x;
    int y;
};

typedef struct position pos_t;

void move_val(pos_t pos);
void move_ptr(pos_t *pos);
void move_kw_val(struct position pos);
void move_kw_ptr(struct position *pos);

static inline int get_x(pos_t pos)
{
    return pos.x;
}

static inline int get_y(struct position const pos)
{
    return pos.y;
}
