/*
** EPITECH PROJECT, 2023
** goto
** File description:
** goto
*/

#include <stdio.h>

// the definition of func is written in assembly language
__asm__(".globl func\n\t"
        ".type func, @function\n\t"
        "func:\n\t"
        ".cfi_startproc\n\t"
        "movl $7, %eax\n\t"
        "ret\n\t"
        ".cfi_endproc");

void shell(int signal)
{
    void *rip;

    asm volatile ("lea (%%rip), %0" : "=r" (rip));
    __asm volatile ("lea (%%rip), %0" : "=r" (rip));
    __asm__ volatile ("lea (%%rip), %0" : "=r" (rip));
}
