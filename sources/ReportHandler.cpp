/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** report handler
*/

#include "ReportHandler.hpp"
#include "Rule.hpp"

#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/StringRef.h>

ReportHandler::ReportHandler(std::string const &path)
    : _file(path)
{
    if (!_file.is_open())
        throw std::runtime_error("Can't open file");
}

void ReportHandler::report(std::string const &str)
{
    _file << str << std::endl;
}

void ReportHandler::reportViolation(coconut::Rule const &rule,
    clang::CompilerInstance &compiler, clang::SourceLocation const &location)
{
    clang::SourceManager &sm = compiler.getSourceManager();
    clang::SourceLocation expansion = sm.getExpansionLoc(location);
    llvm::StringRef filename = sm.getFilename(expansion);
    unsigned int number = sm.getSpellingLineNumber(expansion);

    _file << std::string(filename) << ":" << number << ": " << rule.getIdentifier()
          << std::endl;
}
