/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** C1
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/StringRef.h>

using namespace clang::ast_matchers;

coconut::RuleC1::RuleC1()
    : Rule("MAJOR:C-C1", "conditional block with more than 3 branches")
{
}

void coconut::RuleC1::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        if (auto stmt = result.Nodes.getNodeAs<clang::Stmt>("nested")) {
            if (stmt->getBeginLoc().isMacroID()) {
                // System macros like SLIST_REMOVE would trigger a false
                // positive
                return;
            }
            report.reportViolation(*this, compiler, stmt->getBeginLoc());
        }
    });

    // Match control flow block statements
    auto controlFlow
        = anyOf(ifStmt(), forStmt(), whileStmt(), switchStmt(), doStmt());
    finder.addMatcher(
        stmt(
            isExpansionInMainFile(),
            // A control flow statement
            controlFlow,
            // In another control flow statement
            hasAncestor(stmt(
                controlFlow,
                // In a third control flow statement
                hasAncestor(stmt(controlFlow))
            ))
        )
            .bind("nested"),
        &handler
    );
    finder.matchAST(context);
}
