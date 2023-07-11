/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** style action
*/

#include "CoconutStyleAction.hpp"
#include "Rule.hpp"

#include <clang/Frontend/CompilerInstance.h>

using namespace coconut;

StyleAction::StyleAction()
{
}

std::unique_ptr<clang::ASTConsumer> StyleAction::CreateASTConsumer(
    clang::CompilerInstance &ci, llvm::StringRef inFile)
{
    return std::make_unique<StyleConsumer>(ci, inFile);
}

StyleConsumer::StyleConsumer(
    clang::CompilerInstance &ci, llvm::StringRef inFile)
    : _compiler(ci)
    , _inFile(inFile)
{
}

void StyleConsumer::HandleTranslationUnit(clang::ASTContext &context)
{
    for (Rule const &rule : _action.getRules()) {
        rule.runCheck(_compiler, context);
    }
}
