/*
** EPITECH PROJECT, 2023
** h1
** File description:
** h1
*/

struct pizza {
    int tomato;
    int dough;
    int cheese;
};

union pizza_array {
    int bytes[3];
    struct pizza pizza;
};

typedef struct pizza pizza_t;

enum pizza_type {
    MARGARITA,
    REGINA,
    AMERICAINE,
    FANTASIA
};

#define MACRO_OK
