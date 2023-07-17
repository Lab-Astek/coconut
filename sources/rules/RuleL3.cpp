/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** L3
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

coconut::RuleL3::RuleL3()
    : Rule("MINOR:C-L3", "misplaced spaces")
{
}

void coconut::RuleL3::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    MatchFinder finder;
    // Handles binary operators
    LambdaCallback binary([&](MatchFinder::MatchResult const &result) {
        auto op = result.Nodes.getNodeAs<clang::BinaryOperator>("op");
        if (!op)
            return;
        clang::SourceLocation loc = op->getOperatorLoc();
        if (loc.isMacroID())
            return;
        clang::SourceManager &sm = compiler.getSourceManager();

        clang::SourceLocation lhsLoc = op->getLHS()->getEndLoc();
        clang::SourceLocation rhsLoc = op->getRHS()->getBeginLoc();

        unsigned int lhsLine = sm.getExpansionLineNumber(lhsLoc);
        unsigned int opLine = sm.getExpansionLineNumber(loc);
        unsigned int rhsLine = sm.getExpansionLineNumber(rhsLoc);

        int opLen = op->getOpcodeStr().size();
        char const *before = sm.getCharacterData(loc) - 1;
        char const *after = sm.getCharacterData(loc) + opLen;
        bool lhsOk
            = lhsLine != opLine || (before[0] == ' ' && before[-1] != ' ');
        if (not lhsOk) {
            report.reportViolation(*this, compiler, loc);
        }
        bool rhsOk = rhsLine != opLine || (after[0] == ' ' && after[1] != ' ');
        if (not rhsOk) {
            report.reportViolation(*this, compiler, loc);
        }
    });

    finder.addMatcher(
        binaryOperator(
            // As usual, we don't want to check header files
            isExpansionInMainFile()
        )
            .bind("op"),
        &binary
    );
    finder.matchAST(context);
}
