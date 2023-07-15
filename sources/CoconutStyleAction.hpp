/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** style action
*/

#ifndef COCONUT_STYLE_ACTION_HPP
#define COCONUT_STYLE_ACTION_HPP

#include "Rule.hpp"

#include <clang/Frontend/FrontendAction.h>
#include <memory>

namespace coconut {

class StyleAction : public clang::ASTFrontendAction {
public:
    StyleAction();

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance &compiler, llvm::StringRef inFile) override;

    bool BeginSourceFileAction(clang::CompilerInstance &compiler) override;

    std::vector<std::unique_ptr<Rule>> const &getRules() const
    {
        return _rules;
    }

    ReportHandler &getReportHandler() const
    {
        return *_reportHandler;
    }

    inline static ReportHandler *_reportHandler;

private:
    std::vector<std::unique_ptr<Rule>> _rules;
};

class StyleConsumer : public clang::ASTConsumer {
public:
    StyleConsumer(StyleAction &action, clang::CompilerInstance &compiler,
        llvm::StringRef inFile);

    void HandleTranslationUnit(clang::ASTContext &context) override;

private:
    StyleAction &_action;
    clang::CompilerInstance &_compiler;
    llvm::StringRef _inFile;
};

} // namespace coconut

#endif /* !COCONUT_STYLE_ACTION_HPP */
