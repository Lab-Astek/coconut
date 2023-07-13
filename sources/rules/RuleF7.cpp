/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** F7
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/StringRef.h>

using namespace clang::ast_matchers;

coconut::RuleF7::RuleF7()
    : Rule("MAJOR:C-F7", "structure passed by value")
{
}

void coconut::RuleF7::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        if (auto stmt = result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
            for (unsigned int i = 0; i < stmt->getNumParams(); i++) {
                auto param = stmt->getParamDecl(i);
                if (param->getType()->isStructureOrClassType()) {
                    report.reportViolation(
                        *this, compiler, param->getLocation());
                }
            }
        }
    });

    finder.addMatcher(
        functionDecl(isDefinition(), isExpansionInMainFile()).bind("func"),
        &handler);
    finder.matchAST(context);
}
