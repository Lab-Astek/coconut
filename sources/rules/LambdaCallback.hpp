/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** lambda match callback
*/

#ifndef COCONUT_LAMBDA_CALLBACK_HPP
#define COCONUT_LAMBDA_CALLBACK_HPP

#include <clang/ASTMatchers/ASTMatchFinder.h>

template <typename T>
struct LambdaCallback : public clang::ast_matchers::MatchFinder::MatchCallback {
    T const lambda;
    LambdaCallback(T lambda)
        : lambda(lambda)
    {
    }

    void run(
        clang::ast_matchers::MatchFinder::MatchResult const &result) override
    {
        lambda(result);
    }
};

#endif /* !COCONUT_LAMBDA_CALLBACK_HPP */
