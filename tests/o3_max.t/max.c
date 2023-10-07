/*
** EPITECH PROJECT, 2023
** o3
** File description:
** o3
*/

static void first_static(int *i)
{
    *i -= 1;
}

static void second_static(int *i)
{
    *i -= 1;
}

static void third_static(int *i)
{
    first_static(i);
}

void first(int *i)
{
    second_static(i);
}

void second(int *i)
{
    third_static(i);
}

void third(int *i)
{
    second(i);
}

static void fourth_static(int *i)
{
    third(i);
}

void fourth(int *i)
{
    fourth_static(i);
}

static void fifth_static(int *i)
{
    fourth(i);
}

int main(void)
{
    int i = 0;

    first(&i);
    second(&i);
    third(&i);
    fourth(&i);
    fifth_static(&i);
}
