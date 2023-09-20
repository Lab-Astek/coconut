/*
** EPITECH PROJECT, 2023
** RuleG2.cpp
** File description:
** RuleG2
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Casting.h>

using namespace clang::ast_matchers;

coconut::RuleG2::RuleG2()
    : Rule(
        "MINOR:C-G2",
        "Inside a source file, implementations of functions must be separated "
        "by one and only one empty line."
    )
{
}

bool isComment(clang::SourceLocation line, std::map<unsigned int, clang::RawComment *> const *comments)
{
    for (auto const &comment: *comments) {
        auto commentBeginLocation = comment.second->getBeginLoc();
        auto commentEndLocation = comment.second->getEndLoc();

        if (line >= commentBeginLocation && line <= commentEndLocation)
            return true;
    }
    return false;
}

void coconut::RuleG2::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    MatchFinder finder;
    clang::SourceManager const &sm = context.getSourceManager();

    // First, we will get a list of the comments in the current file
    auto const *comments = context.Comments.getCommentsInFile(sm.getMainFileID());

    if (!comments)
        return;

    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        auto func = result.Nodes.getNodeAs<clang::FunctionDecl>("function");

        if (!func)
            return;

        auto start = sm.getExpansionLoc(func->getBeginLoc());
        auto currentLine = sm.getExpansionLineNumber(start);
        auto beforeIsEmpty = false;

        for (auto i = currentLine - 1; i > 0; i--) {
            auto beforeCurrentLine = sm.translateLineCol(sm.getFileID(start), i, 1);
            char const *sol = sm.getCharacterData(
                sm.translateLineCol(sm.getMainFileID(), i, 1)
            );
            char const *eol = sm.getCharacterData(
                sm.translateLineCol(sm.getMainFileID(), i, UINT_MAX)
            );
            llvm::StringRef line(sol, eol - sol);

            if (isComment(beforeCurrentLine, comments)) {
                beforeIsEmpty = false;
                continue;
            }

            if (not line.empty() && beforeIsEmpty == true) {
                return;
            }

            if (line.empty() && beforeIsEmpty == false) {
                beforeIsEmpty = true;
                continue;
            }

            report.reportViolation(*this, compiler, start);
            return;
        }
    });

    // Match all function definitions in the main file
    finder.addMatcher(
        functionDecl(isDefinition(), isExpansionInMainFile()).bind("function"),
        &handler
    );
    finder.matchAST(context);
}
