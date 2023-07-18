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

/// Checks if there is a space or not before an operator
/// \param sm SourceManager
/// \param loc Location of the operator
/// \param lhs Left hand side of the operator
/// \param expectSpace True if there should be a space
/// \return True if the space is correct
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

/// Checks if there is a space or not after an operator
/// \param sm SourceManager
/// \param loc Location of the operator
/// \param rhs Right hand side of the operator
/// \param opLen Length of the operator
/// \param expectSpace True if there should be a space
/// \return True if the space is correct
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

/// Checks if there is the expected string between two statements
/// \param compiler CompilerInstance
/// \param sm SourceManager
/// \param lhs Left hand side of the operator
/// \param rhsLoc Location of the right hand side
/// \param expected Expected string
/// \return True if the string is correct
static bool checkCorrectSeparation(
    clang::CompilerInstance &compiler, clang::SourceManager &sm,
    clang::Stmt const *lhs, clang::SourceLocation rhsLoc, char const *expected
)
{
    clang::SourceLocation lhsLoc = lhs->getEndLoc();
    unsigned int lhsLine = sm.getExpansionLineNumber(lhsLoc);
    unsigned int rhsLine = sm.getExpansionLineNumber(rhsLoc);

    if (lhsLine != rhsLine || lhsLoc.isMacroID() || rhsLoc.isMacroID())
        return true;

    lhsLoc = clang::Lexer::getLocForEndOfToken(
        lhsLoc, 0, sm, compiler.getLangOpts()
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
    clang::SourceManager &sm = compiler.getSourceManager();
    MatchFinder finder;
    // Handles binary operators
    LambdaCallback binary([&](MatchFinder::MatchResult const &result) {
        auto op = result.Nodes.getNodeAs<clang::BinaryOperator>("op");
        if (!op)
            return;
        clang::SourceLocation loc = op->getOperatorLoc();
        if (loc.isMacroID())
            return;

        int opLen = op->getOpcodeStr().size();
        // Check left and right
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

        bool ok;
        // Check on the correct side depending on the fixity
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

        clang::Expr const *last = call->getCallee();
        if (call->getNumArgs() > 0)
            last = call->getArg(call->getNumArgs() - 1);
        // Check before the right parenthesis
        if (not checkCorrectSpaceBefore(
                sm, call->getRParenLoc(), last, false
            )) {
            report.reportViolation(*this, compiler, call->getRParenLoc());
            return;
        }

        clang::SourceLocation first = call->getRParenLoc();
        if (call->getNumArgs() > 0)
            first = call->getArg(0)->getBeginLoc();
        // Check both sides of the left parenthesis
        if (not checkCorrectSeparation(
                compiler, sm, call->getCallee(), first, "("
            )) {
            report.reportViolation(*this, compiler, first);
            return;
        }

        auto it
            = llvm::zip(call->arguments(), llvm::drop_begin(call->arguments()));

        // Check commas between arguments
        for (auto const &pair : it) {
            auto const &arg = std::get<0>(pair);
            auto const &next = std::get<1>(pair);
            if (not checkCorrectSeparation(
                    compiler, sm, arg, next->getBeginLoc(), ", "
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

        int opLen
            = clang::Lexer::MeasureTokenLength(loc, sm, compiler.getLangOpts());
        bool expectSpace = true;
        if (auto ret = llvm::dyn_cast<clang::ReturnStmt>(op)) {
            if (ret->getRetValue() == nullptr) {
                // No space after empty return: `return;` not `return ;`
                expectSpace = false;
            }
        }
        // Check there is a space after the keyword
        if (not checkCorrectSpaceAfter(sm, loc, op, opLen, expectSpace)) {
            report.reportViolation(*this, compiler, loc);
        }
    });
    // Handles the do-while loops' while keyword
    LambdaCallback doKeyword([&](MatchFinder::MatchResult const &result) {
        auto op = result.Nodes.getNodeAs<clang::DoStmt>("stmt");
        if (!op)
            return;
        clang::SourceLocation loc = op->getWhileLoc();
        if (loc.isMacroID())
            return;

        int opLen
            = clang::Lexer::MeasureTokenLength(loc, sm, compiler.getLangOpts());
        // Check there is a space after the while keyword
        if (not checkCorrectSpaceAfter(sm, loc, op->getCond(), opLen, true)) {
            report.reportViolation(*this, compiler, loc);
        }
    });
    // Handles the ternary operator
    LambdaCallback ternary([&](MatchFinder::MatchResult const &result) {
        auto op = result.Nodes.getNodeAs<clang::ConditionalOperator>("op");
        if (!op)
            return;
        clang::SourceLocation loc = op->getQuestionLoc();
        if (loc.isMacroID())
            return;

        // Check both sides of the question mark
        if (not checkCorrectSpaceBefore(sm, loc, op->getLHS(), true)
            || not checkCorrectSpaceAfter(sm, loc, op->getRHS(), 1, true)) {
            report.reportViolation(*this, compiler, loc);
            return;
        }

        loc = op->getColonLoc();
        // Check both sides of the colon
        if (not checkCorrectSpaceBefore(sm, loc, op->getLHS(), true)
            || not checkCorrectSpaceAfter(sm, loc, op->getRHS(), 1, true)) {
            report.reportViolation(*this, compiler, loc);
        }
    });
    // Handles for loop conditions
    LambdaCallback forLoop([&](MatchFinder::MatchResult const &result) {
        auto stmt = result.Nodes.getNodeAs<clang::ForStmt>("stmt");
        if (!stmt)
            return;

        clang::Stmt const *init = stmt->getInit();
        clang::Stmt const *cond = stmt->getCond();
        clang::Stmt const *inc = stmt->getInc();

        if (init == nullptr || cond == nullptr || inc == nullptr) {
            // Missing a part of the loop will hopefully be another coding
            // style error soon, so we don't need to report it here
            return;
        }

        // Check space after the left parenthesis
        if (not checkCorrectSpaceAfter(
                sm, stmt->getLParenLoc(), init, 1, false
            )) {
            report.reportViolation(*this, compiler, stmt->getLParenLoc());
            return;
        }

        // Check between init and cond
        // Seems like it's " " or "; " depending on the statements...
        // Therefore both are considered correct
        if (not checkCorrectSeparation(
                compiler, sm, init, cond->getBeginLoc(), " "
            )
            and not checkCorrectSeparation(
                compiler, sm, init, cond->getBeginLoc(), "; "
            )) {
            report.reportViolation(*this, compiler, init->getEndLoc());
            return;
        }
        // Check between cond and inc
        if (not checkCorrectSeparation(
                compiler, sm, cond, inc->getBeginLoc(), " "
            )
            and not checkCorrectSeparation(
                compiler, sm, cond, inc->getBeginLoc(), "; "
            )) {
            report.reportViolation(*this, compiler, cond->getEndLoc());
            return;
        }
        // Check space before the right parenthesis
        if (not checkCorrectSpaceBefore(sm, stmt->getRParenLoc(), inc, false)) {
            report.reportViolation(*this, compiler, stmt->getRParenLoc());
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
    finder.addMatcher(doStmt(isExpansionInMainFile()).bind("stmt"), &doKeyword);
    finder.addMatcher(
        conditionalOperator(isExpansionInMainFile()).bind("op"), &ternary
    );
    finder.addMatcher(forStmt(isExpansionInMainFile()).bind("stmt"), &forLoop);
    finder.matchAST(context);
}
