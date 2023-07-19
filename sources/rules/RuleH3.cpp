/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** H3
*/

#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessingRecord.h>

using namespace clang::ast_matchers;

coconut::RuleH3::RuleH3()
    : Rule("MAJOR:C-H3", "Macros must match only one statement, and fit on a single line")
{
}

void coconut::RuleH3::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    for (clang::PreprocessedEntity *entity :
         *(compiler.getPreprocessor().getPreprocessingRecord())) {

        clang::MacroDefinitionRecord *macro
            = llvm::dyn_cast<clang::MacroDefinitionRecord>(entity);

        if (macro == nullptr)
            continue;

        clang::SourceManager &sourceManager = compiler.getSourceManager();
        auto beginloc = macro->getSourceRange().getBegin();
        auto endLoc = macro->getSourceRange().getEnd();

        // we don't want to check macros defined in extern project headers (system headers for exemple)
        if (not sourceManager.isWrittenInMainFile(beginloc))
            continue;

        if (sourceManager.getExpansionLineNumber(beginloc) != sourceManager.getExpansionLineNumber(endLoc))
            report.reportViolation(*this, compiler, beginloc);
    }
}
