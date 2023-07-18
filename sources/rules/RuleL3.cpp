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

static bool checkCorrectSpaceBefore(
    clang::SourceManager &sm, clang::SourceLocation loc, clang::Expr const *lhs
)
{
    clang::SourceLocation lhsLoc = lhs->getEndLoc();
    unsigned int lhsLine = sm.getExpansionLineNumber(lhsLoc);
    unsigned int opLine = sm.getExpansionLineNumber(loc);
    bool lhsDiffLine = lhsLine != opLine || lhsLoc.isMacroID();
    char const *before = sm.getCharacterData(loc) - 1;

    return lhsDiffLine || (before[0] == ' ' && before[-1] != ' ');
}

static bool checkCorrectSpaceAfter(
    clang::SourceManager &sm, clang::SourceLocation loc, clang::Expr const *rhs,
    size_t opLen
)
{
    clang::SourceLocation rhsLoc = rhs->getBeginLoc();
    unsigned int rhsLine = sm.getExpansionLineNumber(rhsLoc);
    unsigned int opLine = sm.getExpansionLineNumber(loc);
    bool rhsDiffLine = rhsLine != opLine || rhsLoc.isMacroID();
    char const *after = sm.getCharacterData(loc) + opLen;

    return rhsDiffLine || (after[0] == ' ' && after[1] != ' ');
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

        int opLen = op->getOpcodeStr().size();
        if (not checkCorrectSpaceBefore(sm, loc, op->getLHS()) ||
            not checkCorrectSpaceAfter(sm, loc, op->getRHS(), opLen)) {
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
