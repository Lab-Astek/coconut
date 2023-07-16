/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** Constants
*/

#ifndef COCONUT_CONTANTS_HPP
#define COCONUT_CONTANTS_HPP

namespace coconut {

constexpr unsigned int MINIMAL_FUNC_NAME_SIZE = 3;

constexpr unsigned int LINE_LIMIT = 20;

constexpr unsigned int PARAMS_LIMIT = 4;

constexpr unsigned int FUNCTION_LIMIT = 5;

constexpr char const *SNAKECASE_REGEX = "[a-z0-9]+(?:_[a-z0-9]+)*";

constexpr char const *GLOBAL_VAR_SNAKECASE_REGEX = "[A-Z0-9]+(?:_[A-Z0-9]+)*";

constexpr char const *TYPEDEF_SNAKECASE_REGEX = "[a-z1-9]+(?:_[a-z1-9]+)*_t";
}

#endif /* !COCONUT_CONTANTS_HPP */
