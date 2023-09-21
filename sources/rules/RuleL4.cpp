/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** l4
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessingRecord.h>
#include <llvm/ADT/StringRef.h>

#include <iostream>

using namespace clang::ast_matchers;

coconut::RuleL4::RuleL4()
    : Rule("MINOR:C-L4", "misplaced brackets")
{
}

static bool bracketOnGoodPosition(
    clang::SourceLocation const &stmtLoc,
    clang::CompoundStmt const *cmpdStmt,
    clang::SourceManager &sm
)
{
    clang::SourceLocation const &BodyLoc = cmpdStmt->getBeginLoc();


    if (sm.getExpansionLineNumber(stmtLoc) != sm.getExpansionLineNumber(BodyLoc))
        return false;

    char const charAfterBracket = sm.getCharacterData(BodyLoc)[1];
    if (charAfterBracket != '\n')
        return false;

    return true;
}

static bool isAloneOnHisLine(clang::SourceLocation const &loc, clang::SourceManager &sm)
{
    clang::SourceLocation beginLoc = sm.translateLineCol(sm.getMainFileID(),sm.getSpellingLineNumber(loc),1);
    char const *charPtr = sm.getCharacterData(beginLoc);
    llvm::StringRef line(charPtr);

    line = line.take_front(line.find('\n'));
    if (line.find_first_not_of(" }\n") != llvm::StringRef::npos)
        return false;
    return true;
}

void coconut::RuleL4::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;

    LambdaCallback functions([&] (MatchFinder::MatchResult const &result) {
        auto func = result.Nodes.getNodeAs<clang::FunctionDecl>("function");

        if (!func)
            return;

        auto body = llvm::dyn_cast<clang::CompoundStmt>(func->getBody());

        if (!body)
            return;

        auto &sm = compiler.getSourceManager();
        clang::SourceLocation loc = body->getBeginLoc();

        unsigned int start = sm.getExpansionLineNumber(loc);
        unsigned int end = sm.getExpansionLineNumber(body->getEndLoc());

        unsigned int first_line_length = sm.getCharacterData(
            sm.translateLineCol(sm.getMainFileID(), start, UINT_MAX)) -
            sm.getCharacterData(sm.translateLineCol(sm.getMainFileID(), start, 1)
        );
        unsigned int end_line_length = sm.getCharacterData(
            sm.translateLineCol(sm.getMainFileID(), end, UINT_MAX)) -
            sm.getCharacterData(sm.translateLineCol(sm.getMainFileID(), end, 1)
        );

        if (first_line_length != 1)
            report.reportViolation(*this, compiler, loc);
        else if (end_line_length != 1)
            report.reportViolation(*this, compiler, body->getEndLoc());
    });

    LambdaCallback cmpdStmt([&] (MatchFinder::MatchResult const &result) {
        auto stmt = result.Nodes.getNodeAs<clang::Stmt>("stmt");

        if (!stmt)
            return;

        auto &sm = compiler.getSourceManager();
        if (auto ifStmt = llvm::dyn_cast<clang::IfStmt>(stmt)) {
            auto const *cmpdStmt = llvm::dyn_cast<clang::CompoundStmt>(ifStmt->getThen());

            if (cmpdStmt) {
                if (not bracketOnGoodPosition(ifStmt->getBeginLoc(), cmpdStmt, sm))
                    report.reportViolation(*this, compiler, ifStmt->getBeginLoc());
                if (not ifStmt->hasElseStorage() and
                    not isAloneOnHisLine(ifStmt->getEndLoc(), sm))
                    report.reportViolation(*this, compiler, ifStmt->getEndLoc());
            }

            if (ifStmt->hasElseStorage()) {
                auto const *elseStmt = llvm::dyn_cast<clang::CompoundStmt>(ifStmt->getElse());

                if (not elseStmt)
                    return;

                if (not bracketOnGoodPosition(ifStmt->getElseLoc(), elseStmt, sm))
                    report.reportViolation(*this, compiler, ifStmt->getElseLoc());
                if (not isAloneOnHisLine(ifStmt->getElse()->getEndLoc(), sm))
                    report.reportViolation(*this, compiler, ifStmt->getElse()->getEndLoc());
            }

        } else if (auto forStmt = llvm::dyn_cast<clang::ForStmt>(stmt)) {
            // not dev yet
        } else if (auto whileStmt = llvm::dyn_cast<clang::WhileStmt>(stmt)) {
            // not dev yet
        } else if (auto doStmt = llvm::dyn_cast<clang::DoStmt>(stmt)) {
            // not dev yet
        } else if (auto switchStmt = llvm::dyn_cast<clang::SwitchStmt>(stmt)) {
            // not dev yet
        }
    });

    finder.addMatcher(
        functionDecl(
            isExpansionInMainFile(),
            isDefinition()
        )
            .bind("function"),
        &functions
    );

    auto controlFlow =
        anyOf(ifStmt(), forStmt(), whileStmt(), switchStmt(), doStmt());

    finder.addMatcher(
        stmt(
            isExpansionInMainFile(),
            controlFlow
        )
            .bind("stmt"),
        &cmpdStmt
    );

    finder.matchAST(context);
}
