/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test L5
*/

#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>

int oh_no_variables_in_the_middle(void)
{
    int ok = 0;

    if (ok == 0) {
        int pid = getpid();
        printf("pid: %d\n", pid);
        return pid;
    }
    int ret = -1;
    return ret;
}

int for_loop_is_ok(void)
{
    for (int i = 0; i < 10; i++) {
        printf("%d\n", i);
    }
    return 0;
}

int main(void)
{
    double d = 0.0;
    float g, h = 1;
    void *a = NULL;
    int b = 0, c = 1;
    char e = 'a'; int f = 1;

    g = 1.0;
    return d + g + h + b + c + e + f;
}

void macros_which_expand_to_var_decl(fd_set *set)
{
    FD_ZERO(set);
    FD_SET(STDIN_FILENO, set);
}
