/*
** EPITECH PROJECT, 2023
** f4
** File description:
** f4
*/

#include <stdio.h>

int flowy(void)
{
    if(1) {
        return(1);
    } else {
        return  (0);
    }
    while(7) {
        printf("hello\n");
    }
    return (0);
}

int flower(void)
{
    for(int i = 0; i < 10; i++) {
        switch(i) {
        case 1:
            return(1);
        default:
            break;
        }
    }
    do{
        return 3;
    } while(0);
}

int main(void)
{
    flowy();
    flower();
}
