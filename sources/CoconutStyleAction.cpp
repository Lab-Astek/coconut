/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** style action
*/

#include "CoconutStyleAction.hpp"
#include "rules/Rules.hpp"

#include <clang/Frontend/CompilerInstance.h>

using namespace coconut;

StyleAction::StyleAction()
{
    _rules.emplace_back(std::make_unique<coconut::RuleF7>());
    _rules.emplace_back(std::make_unique<coconut::RuleC3>());
    _rules.emplace_back(std::make_unique<coconut::RuleF4>());
    _rules.emplace_back(std::make_unique<coconut::RuleO3>());
}

std::unique_ptr<clang::ASTConsumer> StyleAction::CreateASTConsumer(
    clang::CompilerInstance &ci, llvm::StringRef inFile)
{
    return std::make_unique<StyleConsumer>(*this, ci, inFile);
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
