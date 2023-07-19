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

void coconut::RuleG2::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    std::size_t func_nbr = 0;
    unsigned int saveEndLine;
    clang::SourceManager &sm = compiler.getSourceManager();

    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        auto func = result.Nodes.getNodeAs<clang::FunctionDecl>("function");

        if (!func)
            return;
        func_nbr++;

        auto start = func->getBeginLoc();
        auto end = func->getEndLoc();

        if (func_nbr >= 2) {
            unsigned int startLine = sm.getExpansionLineNumber(start);
            if (startLine - saveEndLine != 2)
                report.reportViolation(*this, compiler, start);
        }
        saveEndLine = sm.getExpansionLineNumber(end);
    });

    // Match all function definitions in the main file
    finder.addMatcher(
        functionDecl(isDefinition(), isExpansionInMainFile()).bind("function"),
        &handler
    );
    finder.matchAST(context);
}
