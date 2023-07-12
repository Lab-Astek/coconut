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
    llvm::StringRef filename = sm.getFilename(location);
    unsigned int number = sm.getSpellingLineNumber(location);

    _file << std::string(filename) << ":" << number << ": " << rule.getIdentifier()
          << std::endl;
}

std::optional<clang::SourceLocation> ReportHandler::getExpansionLoc(
    clang::CompilerInstance &compiler,
    clang::SourceLocation const &location)
{
    clang::SourceManager &sm = compiler.getSourceManager();
    clang::SourceLocation expansion = sm.getExpansionLoc(location);
    clang::FileID id = sm.getFileID(expansion);
    clang::FileID file = sm.getMainFileID();

    if (id != file)
        return std::nullopt;
    return expansion;
}
