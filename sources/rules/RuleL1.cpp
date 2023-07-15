/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** L1
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Stmt.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Casting.h>

#include <unordered_set>

using namespace clang::ast_matchers;

coconut::RuleL1::RuleL1()
    : Rule("MAJOR:C-L1", "a line must correspond to only one statement")
{
}

void coconut::RuleL1::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    MatchFinder finder;

    /// The set of lines on which a statement is found
    std::unordered_set<unsigned int> lines_used;

    // TODO doc
    LambdaCallback statements([&](MatchFinder::MatchResult const &result) {
        auto stmt = result.Nodes.getNodeAs<clang::Stmt>("statement");
        if (!stmt)
            return;
        clang::SourceManager &sm = compiler.getSourceManager();

        auto start = stmt->getBeginLoc();
        auto end = stmt->getEndLoc();

        auto handleStmt = [&]() {
            if (start.isMacroID() || end.isMacroID()) {
                // If it's in a (system header) macro expansion,
                // it's probably a false positive (ex: FD_ZERO)
                return;
            }

            unsigned int start_line = sm.getExpansionLineNumber(start);
            unsigned int end_line = sm.getExpansionLineNumber(end);

            for (unsigned int i = start_line; i <= end_line; i++) {
                if (lines_used.find(i) == lines_used.end()) {
                    lines_used.insert(i);
                    continue;
                }
                report.reportViolation(*this, compiler, stmt->getBeginLoc(), i);
                return;
            }
        };

        if (auto ifStmt = llvm::dyn_cast<clang::IfStmt>(stmt)) {
            end = ifStmt->getRParenLoc();
            // Handle the condition part
            handleStmt();
            if (ifStmt->hasElseStorage()
                // "else if" is allowed
                && not llvm::isa<clang::IfStmt>(ifStmt->getElse())) {
                start = end = ifStmt->getElseLoc();
                // Handle the else keyword
                handleStmt();
            }
        } else if (auto forStmt = llvm::dyn_cast<clang::ForStmt>(stmt)) {
            end = forStmt->getRParenLoc();
            // Handle the content of the parenthesis
            handleStmt();
        } else if (auto whileStmt = llvm::dyn_cast<clang::WhileStmt>(stmt)) {
            end = whileStmt->getRParenLoc();
            // Handle the condition part
            handleStmt();
        } else if (auto doStmt = llvm::dyn_cast<clang::DoStmt>(stmt)) {
            start = end = doStmt->getDoLoc();
            // Handle the do keyword
            handleStmt();
            start = doStmt->getWhileLoc();
            end = doStmt->getRParenLoc();
            // Handle the condition part
            handleStmt();
        } else if (auto switchStmt = llvm::dyn_cast<clang::SwitchStmt>(stmt)) {
            end = switchStmt->getRParenLoc();
            // Handle the content of the parenthesis
            handleStmt();
        } else if (llvm::isa<clang::CompoundStmt>(stmt)) {
            // Ignore those, handled by rule L4
        } else {
            handleStmt();
        }
    });

    finder.addMatcher(
        stmt(
            // we don't want to stuff declared in headers
            isExpansionInMainFile(),
            anyOf(
                // If top level in an if/for/while/do/compound statement
                // (but not in the condition of the statement)
                hasParent(ifStmt(hasCondition(expr().bind("cond")))),
                hasParent(forStmt(hasCondition(expr().bind("cond")))),
                hasParent(whileStmt(hasCondition(expr().bind("cond")))),
                hasParent(doStmt(hasCondition(expr().bind("cond")))),
                hasParent(compoundStmt()),
                // If parent is anything else, it's an expression
                // However, assignments are considered statements in this rule
                binaryOperator(isAssignmentOperator())
            ),
            // that's where we check it's not the condition of a statement
            unless(equalsBoundNode("cond")),
            unless(hasParent(forStmt()))
        )
            .bind("statement"),
        &statements
    );
    finder.matchAST(context);
}
