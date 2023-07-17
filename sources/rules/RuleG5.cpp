/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** G5
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessingRecord.h>

#include <iostream>

using namespace clang::ast_matchers;

coconut::RuleG5::RuleG5()
    : Rule("MAJOR:C-G5", "include directive must only include C header (.h) files")
{
}

void coconut::RuleG5::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    clang::SourceManager &sourceManager = compiler.getSourceManager();

    for (clang::PreprocessedEntity *entity: *(compiler.getPreprocessor().getPreprocessingRecord())) {

        clang::InclusionDirective *inclusion = llvm::dyn_cast<clang::InclusionDirective>(entity);

        if (inclusion == nullptr)
            continue;

        auto loc = inclusion->getSourceRange().getEnd();

        if (not sourceManager.isWrittenInMainFile(loc))
            continue;

        auto const &fileName = inclusion->getFileName();
        auto fileExtension = fileName.slice(fileName.find('.'), fileName.size()).str();

        if (fileExtension.compare(".h") != 0)
            report.reportViolation(*this, compiler, loc);
    }

}
