/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** F2
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/Support/Casting.h>

#include <regex>
#include <iostream>

using namespace clang::ast_matchers;

static constexpr unsigned int minimalVarSize = 3;

coconut::RuleF2::RuleF2()
    : Rule("MINOR:C-F2", "functions name must match snake_case convention and be in English")
{
}

void coconut::RuleF2::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;

    LambdaCallback handler([&] (MatchFinder::MatchResult const &result) {
        auto func = result.Nodes.getNodeAs<clang::FunctionDecl>("function");
        std::regex const snakeCaseRegex("[a-z]+(?:_[a-z]+)*");

        if (not func)
            return;

        // Check if the function name doesn't respect the snake_case convention
        if (not std::regex_match(func->getName().str(), snakeCaseRegex))
            report.reportViolation(*this, compiler, func->getLocation());
        // Also, we check if the function name is not shorter than 3 characters
        else if (func->getName().str().size() < minimalVarSize)
            report.reportViolation(*this, compiler, func->getLocation());

    });

    finder.addMatcher(
        functionDecl(
            // we only want functions definitions
            isDefinition(),
            // so, we don't want to match any function declared in headers
            isExpansionInMainFile()
        ).bind("function"),
        &handler
    );

    finder.matchAST(context);
}
