/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** O4
*/

#include "Constants.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/CompilerInstance.h>

#include <regex>

using namespace clang::ast_matchers;

coconut::RuleO4::RuleO4()
    : Rule("MINOR:C-O4", "file names must be in snake_case convention")
{
}

void coconut::RuleO4::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    [[maybe_unused]] clang::ASTContext &context
) const
{
    clang::SourceManager &sm = compiler.getSourceManager();
    std::string filename = sm.getFileEntryForID(sm.getMainFileID())->getName().str();
    std::regex const snakeCaseReg(coconut::SNAKECASE_REGEX);

    filename = filename.substr(filename.find_last_of('/') + 1);
    filename = filename.substr(0, filename.find_last_of('.'));

    if (not std::regex_match(filename, snakeCaseReg))
        report.reportViolation(*this, compiler, sm.getLocForStartOfFile(sm.getMainFileID()));
}
