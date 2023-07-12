/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** O3
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>

using namespace clang::ast_matchers;

static constexpr std::size_t function_limit = 5;

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
        if (auto func = result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
            auto loc = ReportHandler::getExpansionLoc(compiler, func->getLocation());

            if (not loc)
                return;

            func_nbr++;
            if (func_nbr > function_limit)
                report.reportViolation(*this, compiler, *loc);
        }
    });

    finder.addMatcher(functionDecl(isDefinition()).bind("function"), &handler);
    finder.matchAST(context);
}
