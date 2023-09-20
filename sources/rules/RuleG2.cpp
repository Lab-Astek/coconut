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

bool isComment(const char *str)
{
    if (strncmp(str, "//", 2) == 0)
        return true;
    if (strncmp(str, "/*", 2) == 0)
        return true;
    return false;
}

void coconut::RuleG2::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        auto func = result.Nodes.getNodeAs<clang::FunctionDecl>("function");

        if (!func)
            return;

        auto &sm = result.Context->getSourceManager();
        auto start = sm.getExpansionLoc(func->getBeginLoc());
        auto currentLine = sm.getExpansionLineNumber(start);
        
        auto beforeIsEmpty = false;

        for (auto i = currentLine - 1; i > 0; i--) {
            auto beforeCurrentLine = sm.translateLineCol(sm.getFileID(start), i, 1);
            auto beforeCurrentLineStr = sm.getCharacterData(beforeCurrentLine);

            if (isComment(beforeCurrentLineStr)) {
                beforeIsEmpty = false;
                continue;
            }

            if (strncmp(beforeCurrentLineStr, "\n", 1) != 0) {
                beforeIsEmpty = false;
                break;
            }

            if (beforeIsEmpty == false && strncmp(beforeCurrentLineStr, "\n", 1) == 0) {
                beforeIsEmpty = true;
                continue;
            }
            report.reportViolation(*this, compiler, start);
        }
    });

    // Match all function definitions in the main file
    finder.addMatcher(
        functionDecl(isDefinition(), isExpansionInMainFile()).bind("function"),
        &handler
    );
    finder.matchAST(context);
}
