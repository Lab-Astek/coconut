/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** F5
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>

#include <iostream>

using namespace clang::ast_matchers;

static constexpr std::size_t params_limit = 4;

coconut::RuleF5::RuleF5()
    : Rule("MAJOR:C-F5", "function should not have more than 4 parameters")
{
}

void coconut::RuleF5::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        if (auto func = result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
            auto loc = ReportHandler::getExpansionLoc(compiler, func->getLocation());
            unsigned int nbParams = func->getNumParams();

            if (not loc)
                return;

            if (nbParams > params_limit)
                report.reportViolation(*this, compiler, *loc);
        }
    });

    finder.addMatcher(functionDecl(isDefinition()).bind("function"), &handler);
    finder.matchAST(context);

}
