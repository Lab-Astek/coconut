/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** test g4
*/

#pragma once

#define F_FMT "%f"
#define D_FMT "%f"
#define LD_FMT "%lf"

#define FPD_FMT(a) ((sizeof(a) == sizeof(double)) ? D_FMT : LD_FMT)
#define FP_FMT(a) ((sizeof(a) == sizeof(float)) ? F_FMT : FPD_FMT(a))
