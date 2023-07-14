/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** F4
*/

#include "Constants.hpp"
#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/CompilerInstance.h>

using namespace clang::ast_matchers;

coconut::RuleF4::RuleF4()
    : Rule("MAJOR:C-F4", "function should not exceed 20 lines")
{
}

void coconut::RuleF4::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        if (auto func
            = result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
            clang::SourceManager &sm = compiler.getSourceManager();

            clang::SourceLocation begin = func->getBody()->getBeginLoc();
            clang::SourceLocation end = func->getBody()->getEndLoc();

            unsigned int start_number = sm.getSpellingLineNumber(begin);
            unsigned int end_number = sm.getSpellingLineNumber(end);

            // Report a violation on each line that exceeds the limit
            for (unsigned int i = start_number + coconut::LINE_LIMIT + 1;
                 i < end_number; ++i)
                report.reportViolation(*this, compiler, begin, i);
        }
    });

    // Match all function definitions in the main file
    finder.addMatcher(
        functionDecl(isDefinition(), isExpansionInMainFile()).bind("function"),
        &handler
    );
    finder.matchAST(context);
}
