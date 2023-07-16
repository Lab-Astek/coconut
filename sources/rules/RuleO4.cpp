/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** O4
*/

#include "Constants.hpp"
#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/CompilerInstance.h>

using namespace clang::ast_matchers;

coconut::RuleO4::RuleO4()
    : Rule("MINOR:C-O4", "file names and folders must be in English, according to the snake_case convention")
{
}

void coconut::RuleO4::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
}
