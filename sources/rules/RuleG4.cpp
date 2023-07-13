/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** G4
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

coconut::RuleG4::RuleG4()
    : Rule("MAJOR:C-G4", "global variables are forbidden")
{
}

void coconut::RuleG4::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        if (auto stmt = result.Nodes.getNodeAs<clang::VarDecl>("var")) {
            report.reportViolation(*this, compiler, stmt->getLocation());
        }
    });

    auto globalVar = varDecl(isDefinition(), hasStaticStorageDuration(),
        isExpansionInMainFile(), unless(hasType(isConstQualified())),
        hasParent(translationUnitDecl()));
    finder.addMatcher(globalVar.bind("var"), &handler);
    finder.matchAST(context);
}
