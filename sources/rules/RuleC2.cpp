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

void coconut::RuleC2::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    MatchFinder finder;
    // Handles ternary operator in a ternary operator (nested or chained)
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        auto parent
            = result.Nodes.getNodeAs<clang::ConditionalOperator>("parent");
        auto child
            = result.Nodes.getNodeAs<clang::ConditionalOperator>("child");
        if (!parent || !child)
            return;
        if (parent->getBeginLoc().isMacroID()
            || child->getBeginLoc().isMacroID())
            return;
        report.reportViolation(*this, compiler, parent->getBeginLoc());
    });
    // Handles ternary operator as expression-statement
    LambdaCallback stmtHandler([&](MatchFinder::MatchResult const &result) {
        auto ternary
            = result.Nodes.getNodeAs<clang::ConditionalOperator>("ternary");
        if (!ternary)
            return;
        if (ternary->getBeginLoc().isMacroID())
            return;
        report.reportViolation(*this, compiler, ternary->getBeginLoc());
    });

    finder.addMatcher(
        conditionalOperator(
            // As usual, we don't want to check header files
            isExpansionInMainFile(),
            // Check for nested ternary operator(in both branches)
            hasDescendant(conditionalOperator().bind("child"))
        )
            .bind("parent"),
        &handler
    );
    finder.addMatcher(
        conditionalOperator(
            // As usual, we don't want to check header files
            isExpansionInMainFile(),
            anyOf(
                // If top level in an if/for/while/do/compound statement
                // (but not in the condition of the statement)
                hasParent(ifStmt(hasCondition(expr().bind("cond")))),
                hasParent(forStmt(hasCondition(expr().bind("cond")))),
                hasParent(whileStmt(hasCondition(expr().bind("cond")))),
                hasParent(doStmt(hasCondition(expr().bind("cond")))),
                hasParent(compoundStmt())
                // If parent is anything else, it's allowed
            ),
            // that's where we check it's not the condition of a statement
            unless(equalsBoundNode("cond"))
        )
            .bind("ternary"),
        &stmtHandler
    );
    finder.matchAST(context);
}
