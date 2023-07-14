/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** O3
*/

#include "Constants.hpp"
#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/CompilerInstance.h>

using namespace clang::ast_matchers;

coconut::RuleO3::RuleO3()
    : Rule("MAJOR:C-O3", "too many function declaration")
{
}

void coconut::RuleO3::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    std::size_t func_nbr = 0;

    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        if (auto func
            = result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
            func_nbr++;
            if (func_nbr > coconut::FUNCTION_LIMIT)
                report.reportViolation(*this, compiler, func->getLocation());
        }
    });

    finder.addMatcher(
        functionDecl(isDefinition(), isExpansionInMainFile()).bind("function"),
        &handler);
    finder.matchAST(context);
}
