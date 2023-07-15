/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** style action
*/

#include "CoconutStyleAction.hpp"
#include "rules/Rules.hpp"

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/PreprocessorOptions.h>

using namespace coconut;

StyleAction::StyleAction()
{
#define DECLARE_RULE(name)                                                     \
    _rules.emplace_back(std::make_unique<coconut::Rule##name>());
#include "rules/Rules.inc"
#undef DECLARE_RULE
}

std::unique_ptr<clang::ASTConsumer> StyleAction::CreateASTConsumer(
    clang::CompilerInstance &ci, llvm::StringRef inFile)
{
    return std::make_unique<StyleConsumer>(*this, ci, inFile);
}

bool StyleAction::BeginSourceFileAction(clang::CompilerInstance &compiler)
{
    compiler.getPreprocessor().getPreprocessorOpts().DetailedRecord = 1;

    return true;
}

StyleConsumer::StyleConsumer(
    StyleAction &action, clang::CompilerInstance &ci, llvm::StringRef inFile)
    : _action(action)
    , _compiler(ci)
    , _inFile(inFile)
{
}

void StyleConsumer::HandleTranslationUnit(clang::ASTContext &context)
{
    for (auto const &rule : _action.getRules()) {
        rule->runCheck(_action.getReportHandler(), _compiler, context);
    }
}
