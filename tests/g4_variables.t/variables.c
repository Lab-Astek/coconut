/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test g4
*/

#include "variables.h"

int myExternVar = 3;

int main(void)
{
    static int staticVarOK = 3;
    myGlobalVar = -1;
    myStaticVar = 3;
    myTentativeVar = 8;
    myExternVar = 84;
    return myGlobalVar;
}
