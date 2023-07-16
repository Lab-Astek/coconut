/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test L5
*/

#pragma once

void *my_ok_func(char const *str);
void*missing_space(char *str);
void* attached_wrongly(char *str);
void * extra_space(char *str);

void *param_missing_space(char *str);
void *param_attached_wrongly(char *str);
void *param_extra_space(char * str);

int main_ok(int argc, char **argv);
int main_missing_space(int argc, char**argv);
int main_attached_wrongly(int argc, char** argv);
int main_extra_space(int argc, char ** argv);

int main2_ok(int argc, char *argv[]);
int main2_missing_space(int argc, char*argv[]);
int main2_attached_wrongly(int argc, char* argv[]);
int main2_extra_space(int argc, char * argv[]);
