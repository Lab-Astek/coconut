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
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/CompilerInstance.h>

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
        if (auto func
            = result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
            unsigned int nbParams = func->getNumParams();
            if (nbParams > params_limit)
                report.reportViolation(*this, compiler, func->getLocation());
        }
    });

    finder.addMatcher(functionDecl(isDefinition(), isExpansionInMainFile()).bind("function"), &handler);
    finder.matchAST(context);
}
