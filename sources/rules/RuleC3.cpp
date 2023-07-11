/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** C3
*/

#include "Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <iostream>
#include <llvm/ADT/StringRef.h>

using namespace clang::ast_matchers;
using clang::CompilerInstance;

class GotoStmtHandler : public MatchFinder::MatchCallback {
    coconut::RuleC3 const &_rule;
    CompilerInstance &_compiler;
    ReportHandler &_report;

public:
    GotoStmtHandler(coconut::RuleC3 const &rule, CompilerInstance &compiler,
        ReportHandler &report)
        : _rule(rule)
        , _compiler(compiler)
        , _report(report)
    {
    }

    void run(MatchFinder::MatchResult const &result) override
    {
        if (auto const *stmt
            = result.Nodes.getNodeAs<clang::GotoStmt>("goto")) {
            _report.reportViolation(_rule, _compiler, stmt->getGotoLoc());
        }
    }
};

coconut::RuleC3::RuleC3()
    : Rule("MAJOR:C-C3", "goto statement is forbidden")
{
}

void coconut::RuleC3::runCheck(ReportHandler &report,
    CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;
    GotoStmtHandler handler(*this, compiler, report);

    finder.addMatcher(gotoStmt().bind("goto"), &handler);
    finder.matchAST(context);
}
