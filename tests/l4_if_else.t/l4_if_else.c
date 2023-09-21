/*
** EPITECH PROJECT, 2023
** l4_if_else
** File description:
** l4_if_else
*/

int else_test(void)
{
    int a = 2;

    if (a == 1)
        a = 3;
    else
    {
        a = 3;
        return -1;
    }
    return 0;
}

int main(void)
{
    int i = 0;

    if (i == 0)
    {
        return i;
    } else {
        i = 2; }
    if (i == 1) { return 0; }
    if (else_test() == -1) {
        return 0;
    } return 0;
}
