/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** G10
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Stmt.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/StringRef.h>

using namespace clang::ast_matchers;

coconut::RuleG10::RuleG10()
    : Rule("FATAL:C-G10", "inline assembly is forbidden")
{
}

namespace {
const clang::ast_matchers::internal::VariadicDynCastAllOfMatcher<
    clang::Decl, clang::FileScopeAsmDecl>
    fileScopeAsmDecl;
    // asmStmt doesn't work, but GCCAsmStmt does
const clang::ast_matchers::internal::VariadicDynCastAllOfMatcher<
    clang::Stmt, clang::GCCAsmStmt>
    gccAsmStmt;
}

void coconut::RuleG10::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        if (auto stmt = result.Nodes.getNodeAs<clang::AsmStmt>("asm-stmt")) {
            report.reportViolation(*this, compiler, stmt->getAsmLoc());
        } else if (auto decl = result.Nodes.getNodeAs<clang::FileScopeAsmDecl>("asm-decl")) {
            report.reportViolation(*this, compiler, decl->getAsmLoc());
        }
    });

    // Match all statements in functions in the main file
    finder.addMatcher(
        gccAsmStmt(isExpansionInMainFile()).bind("asm-stmt"), &handler
    );
    // Match top level asm declarations in the main file
    finder.addMatcher(
        fileScopeAsmDecl(isExpansionInMainFile()).bind("asm-decl"), &handler
    );
    finder.matchAST(context);
}
