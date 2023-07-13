/*
** EPITECH PROJECT, 2023
** o3
** File description:
** o3
*/

void first(int *a, int *b, int *c, int *d, int *e, int *f, int *g, int *h)
{
    *a -= 1;
}

void sec(int *a, int *b, int *c, int *d, int *e, int *f, int *g, int *h)
{
    *a -= 1;
}

int main(void)
{
    int i = 0;

    first(&i, &i, &i, &i, &i, &i, &i, &i);
    sec(&i, &i, &i, &i, &i, &i, &i, &i);
}
