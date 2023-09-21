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

#include <iostream>

using namespace clang::ast_matchers;

coconut::RuleL4::RuleL4()
    : Rule("MINOR:C-L4", "misplaced brackets")
{
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

        if (auto ifStmt = llvm::dyn_cast<clang::IfStmt>(stmt)) {
            if (not llvm::dyn_cast<clang::CompoundStmt>(ifStmt->getThen()))
                return;

            auto &sm = compiler.getSourceManager();
            clang::SourceLocation const &ifLoc = ifStmt->getBeginLoc();
            clang::SourceLocation const &ifBodyLoc = ifStmt->getThen()->getBeginLoc();


            if (sm.getExpansionLineNumber(ifLoc) != sm.getExpansionLineNumber(ifBodyLoc)) {
                report.reportViolation(*this, compiler, ifLoc);
                return;
            }

            char const charAfterBracket = sm.getCharacterData(ifBodyLoc)[1];
            if (charAfterBracket != '\n')
                report.reportViolation(*this, compiler, ifLoc);

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
