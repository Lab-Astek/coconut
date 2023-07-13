/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** F6
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/Support/Casting.h>

using namespace clang::ast_matchers;

coconut::RuleF6::RuleF6()
    : Rule("MAJOR:C-F6", "function without parameters should take void as his only parameter")
{
}

void coconut::RuleF6::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;
    LambdaCallback handler([&] (MatchFinder::MatchResult const &result) {
        if (auto func = result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
            if (not func->hasPrototype())
                report.reportViolation(*this, compiler, func->getLocation());
        }
    });

    finder.addMatcher(
        functionDecl(
            isDefinition(),
            isExpansionInMainFile()
        ).bind("function"),
        &handler
    );
    finder.matchAST(context);
}
