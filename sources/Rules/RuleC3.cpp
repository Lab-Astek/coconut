/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** C3
*/

#include "LambdaCallback.hpp"
#include "Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/StringRef.h>

using namespace clang::ast_matchers;

coconut::RuleC3::RuleC3()
    : Rule("MAJOR:C-C3", "goto statement is forbidden")
{
}

void coconut::RuleC3::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        if (auto stmt = result.Nodes.getNodeAs<clang::GotoStmt>("goto")) {
            if (auto loc = ReportHandler::getExpansionLoc(
                    compiler, stmt->getGotoLoc())) {
                report.reportViolation(*this, compiler, *loc);
            }
        }
    });

    finder.addMatcher(gotoStmt().bind("goto"), &handler);
    finder.matchAST(context);
}
