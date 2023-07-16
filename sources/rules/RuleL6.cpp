/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** L6
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Casting.h>

#include <unordered_set>

using namespace clang::ast_matchers;

coconut::RuleL6::RuleL6()
    : Rule(
        "MINOR:C-L6",
        "a line break must separate the variable declarations from the code"
    )
{
}

void coconut::RuleL6::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    MatchFinder finder;

    // This matcher will catch all the functions in the file, to check their
    // content
    LambdaCallback functions([&](MatchFinder::MatchResult const &result) {
        auto stmt = result.Nodes.getNodeAs<clang::FunctionDecl>("function");
        if (!stmt)
            return;

        auto body = llvm::dyn_cast<clang::CompoundStmt>(stmt->getBody());
        if (!body)
            return;

        auto &sm = compiler.getSourceManager();
        std::optional<unsigned int> separatingLine;
        // Iterate over the initial variable declarations
        for (auto it = body->body_begin(); it != body->body_end(); ++it) {
            auto decl = llvm::dyn_cast<clang::DeclStmt>(*it);
            if (!decl)
                break;

            for (auto declIt = decl->decl_begin(); declIt != decl->decl_end();
                 ++declIt) {
                auto varDecl = llvm::dyn_cast<clang::VarDecl>(*declIt);
                if (!varDecl)
                    break;
                separatingLine
                    = sm.getExpansionLineNumber(varDecl->getEndLoc()) + 1;
            }
        }
        clang::SourceLocation loc = body->getBeginLoc();
        // The first line of the function
        unsigned int start = sm.getExpansionLineNumber(loc) + 1;
        // The one-past-end line of the function (curly brace)
        unsigned int end = sm.getExpansionLineNumber(body->getEndLoc());

        // Check the content of each line of the function
        // Only the separating line should be blank
        for (unsigned int i = start; i < end; ++i) {
            char const *sol = sm.getCharacterData(
                sm.translateLineCol(sm.getMainFileID(), i, 1)
            );
            char const *eol = sm.getCharacterData(
                sm.translateLineCol(sm.getMainFileID(), i, UINT_MAX)
            );
            bool isBlank = (sol == eol);
            bool shouldBeBlank = (i == separatingLine);
            if (isBlank != shouldBeBlank) {
                report.reportViolation(*this, compiler, loc, i);
            }
        }
    });

    finder.addMatcher(
        functionDecl(
            // we don't want to catch functions declared in headers
            isExpansionInMainFile(),
            // we don't care about function prototypes
            isDefinition()
        )
            .bind("function"),
        &functions
    );
    finder.matchAST(context);
}
