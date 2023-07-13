/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** C2
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/StringRef.h>

using namespace clang::ast_matchers;

coconut::RuleC2::RuleC2()
    : Rule("MAJOR:C-C2", "misuse of ternary operator")
{
}

void coconut::RuleC2::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        auto parent = result.Nodes
            .getNodeAs<clang::ConditionalOperator>("parent");
        auto child = result.Nodes
            .getNodeAs<clang::ConditionalOperator>("child");
        if (!parent || !child)
            return;
        if (parent->getBeginLoc().isMacroID() ||
            child->getBeginLoc().isMacroID())
            return;
        report.reportViolation(*this, compiler, parent->getBeginLoc());
    });

    finder.addMatcher(
        conditionalOperator(
            isExpansionInMainFile(),
            hasDescendant(conditionalOperator().bind("child"))
            )
        .bind("parent"),
        &handler);
    finder.matchAST(context);
}
