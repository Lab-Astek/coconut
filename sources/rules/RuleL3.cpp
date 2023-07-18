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
#include <clang/AST/Stmt.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Lexer.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Casting.h>

using namespace clang::ast_matchers;

coconut::RuleL3::RuleL3()
    : Rule("MINOR:C-L3", "misplaced spaces")
{
}

static bool checkCorrectSpaceBefore(
    clang::SourceManager &sm, clang::SourceLocation loc, clang::Stmt const *lhs,
    bool expectSpace
)
{
    clang::SourceLocation lhsLoc = lhs->getEndLoc();
    unsigned int lhsLine = sm.getExpansionLineNumber(lhsLoc);
    unsigned int opLine = sm.getExpansionLineNumber(loc);
    bool lhsDiffLine = lhsLine != opLine || lhsLoc.isMacroID();

    if (lhsDiffLine)
        return true;

    char const *before = sm.getCharacterData(loc) - 1;

    if (expectSpace)
        return (before[0] == ' ' && before[-1] != ' ');
    else
        return (before[0] != ' ');
}

static bool checkCorrectSpaceAfter(
    clang::SourceManager &sm, clang::SourceLocation loc, clang::Stmt const *rhs,
    size_t opLen, bool expectSpace
)
{
    clang::SourceLocation rhsLoc = rhs->getBeginLoc();
    unsigned int rhsLine = sm.getExpansionLineNumber(rhsLoc);
    unsigned int opLine = sm.getExpansionLineNumber(loc);
    bool rhsDiffLine = rhsLine != opLine || rhsLoc.isMacroID();

    if (rhsDiffLine)
        return true;

    char const *after = sm.getCharacterData(loc) + opLen;

    if (expectSpace)
        return (after[0] == ' ' && after[1] != ' ');
    else
        return (after[0] != ' ');
}

static bool checkCorrectSeparation(
    clang::SourceManager &sm, clang::Expr const *lhs,
    clang::SourceLocation rhsLoc, char const *expected
)
{
    clang::SourceLocation lhsLoc = lhs->getEndLoc();
    unsigned int lhsLine = sm.getExpansionLineNumber(lhsLoc);
    unsigned int rhsLine = sm.getExpansionLineNumber(rhsLoc);

    if (lhsLine != rhsLine || lhsLoc.isMacroID() || rhsLoc.isMacroID())
        return true;

    lhsLoc = clang::Lexer::getLocForEndOfToken(
        lhsLoc, 0, sm, clang::LangOptions()
    );
    char const *start = sm.getCharacterData(lhsLoc);
    char const *end = sm.getCharacterData(rhsLoc);

    return llvm::StringRef(start, end - start) == expected;
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
        if (not checkCorrectSpaceBefore(
                sm, loc, op->getLHS(), not op->isCommaOp()
            )
            || not checkCorrectSpaceAfter(sm, loc, op->getRHS(), opLen, true)) {
            report.reportViolation(*this, compiler, loc);
        }
    });
    // Handles unary operators
    LambdaCallback unary([&](MatchFinder::MatchResult const &result) {
        auto op = result.Nodes.getNodeAs<clang::UnaryOperator>("op");
        if (!op)
            return;
        clang::SourceLocation loc = op->getOperatorLoc();
        if (loc.isMacroID())
            return;
        clang::SourceManager &sm = compiler.getSourceManager();

        bool ok;
        if (op->isPostfix()) {
            ok = checkCorrectSpaceBefore(sm, loc, op->getSubExpr(), false);
        } else {
            int opLen
                = clang::UnaryOperator::getOpcodeStr(op->getOpcode()).size();
            ok = checkCorrectSpaceAfter(
                sm, loc, op->getSubExpr(), opLen, false
            );
        }
        if (not ok) {
            report.reportViolation(*this, compiler, loc);
        }
    });
    // Handles function calls
    LambdaCallback call([&](MatchFinder::MatchResult const &result) {
        auto call = result.Nodes.getNodeAs<clang::CallExpr>("call");
        if (!call)
            return;
        clang::SourceManager &sm = compiler.getSourceManager();

        clang::Expr const *last = call->getCallee();
        if (call->getNumArgs() > 0)
            last = call->getArg(call->getNumArgs() - 1);
        if (not checkCorrectSpaceBefore(
                sm, call->getRParenLoc(), last, false
            )) {
            report.reportViolation(*this, compiler, call->getRParenLoc());
            return;
        }

        clang::SourceLocation first = call->getRParenLoc();
        if (call->getNumArgs() > 0)
            first = call->getArg(0)->getBeginLoc();
        if (not checkCorrectSeparation(sm, call->getCallee(), first, "(")) {
            report.reportViolation(*this, compiler, first);
            return;
        }

        auto it
            = llvm::zip(call->arguments(), llvm::drop_begin(call->arguments()));

        for (auto const &pair : it) {
            auto const &arg = std::get<0>(pair);
            auto const &next = std::get<1>(pair);
            if (not checkCorrectSeparation(
                    sm, arg, next->getBeginLoc(), ", "
                )) {
                report.reportViolation(*this, compiler, next->getBeginLoc());
            }
        }
    });
    // Handles statements starting with a keyword
    LambdaCallback keyword([&](MatchFinder::MatchResult const &result) {
        auto op = result.Nodes.getNodeAs<clang::Stmt>("stmt");
        if (!op)
            return;
        clang::SourceLocation loc = op->getBeginLoc();
        if (loc.isMacroID())
            return;
        clang::SourceManager &sm = compiler.getSourceManager();

        int opLen
            = clang::Lexer::MeasureTokenLength(loc, sm, clang::LangOptions());
        bool expectSpace = true;
        if (auto ret = llvm::dyn_cast<clang::ReturnStmt>(op)) {
            if (ret->getRetValue() == nullptr) {
                expectSpace = false;
            }
        }
        if (not checkCorrectSpaceAfter(sm, loc, op, opLen, expectSpace)) {
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
    finder.addMatcher(
        unaryOperator(
            // As usual, we don't want to check header files
            isExpansionInMainFile()
        )
            .bind("op"),
        &unary
    );
    finder.addMatcher(
        callExpr(
            // As usual, we don't want to check header files
            isExpansionInMainFile()
        )
            .bind("call"),
        &call
    );
    finder.addMatcher(
        stmt(anyOf(
            ifStmt(isExpansionInMainFile()).bind("stmt"),
            whileStmt(isExpansionInMainFile()).bind("stmt"),
            forStmt(isExpansionInMainFile()).bind("stmt"),
            returnStmt(isExpansionInMainFile()).bind("stmt"),
            switchStmt(isExpansionInMainFile()).bind("stmt"),
            caseStmt(isExpansionInMainFile()).bind("stmt"),
            doStmt(isExpansionInMainFile()).bind("stmt")
        )),
        &keyword
    );
    finder.matchAST(context);
}
