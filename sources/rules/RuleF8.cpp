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

    // First, we will get all the comments in the current file and stock their location in a vector
    auto const *comments = context.Comments.getCommentsInFile(sourceManager.getMainFileID());
    std::vector<clang::SourceLocation> commentsPos;

    if (comments == nullptr)
        return;

    // Push all comments to the vector
    for (auto const &comment: *comments)
        commentsPos.push_back(comment.second->getBeginLoc());

    // Here the handler will check if a comment is inside any function definition
    // using his location stocked in the vector
    LambdaCallback handler([&] (MatchFinder::MatchResult const &result) {
        auto func = result.Nodes.getNodeAs<clang::FunctionDecl>("function");

        if (!func)
            return;

        auto body = llvm::dyn_cast<clang::CompoundStmt>(func->getBody());

        if (!body)
            return;

        std::size_t funcStart = sourceManager.getExpansionLineNumber(body->getBeginLoc());
        std::size_t funcEnd = sourceManager.getExpansionLineNumber(body->getEndLoc());

        for (auto &comment: commentsPos) {
            if (funcStart <= sourceManager.getExpansionLineNumber(comment)
                and sourceManager.getExpansionLineNumber(comment) < funcEnd)
                report.reportViolation(*this, compiler, comment);
        }

    });

    finder.addMatcher(
        functionDecl(
            // we don't want to catch functions declared in headers
            isExpansionInMainFile(),
            // we don't care about functions declaration
            isDefinition()
        )
            .bind("function"),
        &handler
    );

    finder.matchAST(context);
}
