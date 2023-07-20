/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** f8
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessingRecord.h>

using namespace clang::ast_matchers;

coconut::RuleF8::RuleF8()
    : Rule("MINOR:C-F8", "no comment within a function")
{
}

void coconut::RuleF8::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;
    clang::SourceManager const &sourceManager = context.getSourceManager();

    // First, we will get a list of the comments in the current file
    auto const *comments = context.Comments.getCommentsInFile(sourceManager.getMainFileID());

    if (!comments)
        return;

    // Here the handler will check if a comment is inside any function definition
    // using his location stocked before
    LambdaCallback handler([&] (MatchFinder::MatchResult const &result) {
        auto func = result.Nodes.getNodeAs<clang::FunctionDecl>("function");

        if (!func)
            return;

        auto body = llvm::dyn_cast<clang::CompoundStmt>(func->getBody());

        if (!body)
            return;

        std::size_t funcStart = sourceManager.getExpansionLineNumber(body->getBeginLoc());
        std::size_t funcEnd = sourceManager.getExpansionLineNumber(body->getEndLoc());

        for (auto const &comment: *comments) {
            auto commentLocation = comment.second->getBeginLoc();

            if (funcStart <= sourceManager.getExpansionLineNumber(commentLocation)
                and sourceManager.getExpansionLineNumber(commentLocation) < funcEnd)
                report.reportViolation(*this, compiler, commentLocation);
        }

    });

    finder.addMatcher(
        functionDecl(
            // we don't want to catch functions declared in headers
            isExpansionInMainFile(),
            // we don't care about functions prototype
            isDefinition()
        )
            .bind("function"),
        &handler
    );

    finder.matchAST(context);
}
