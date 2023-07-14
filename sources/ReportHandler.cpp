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
#include <llvm/Support/FileSystem.h>
#include <system_error>

ReportHandler::ReportHandler(std::string const &path)
{
    std::error_code ec;
    _file = std::make_unique<llvm::raw_fd_ostream>(path, ec, llvm::sys::fs::OF_Append);
    if (ec)
        throw std::system_error(ec);
}

void ReportHandler::report(std::string const &str)
{
    *_file << str << '\n';
}

void ReportHandler::reportViolation(coconut::Rule const &rule,
    clang::CompilerInstance &compiler, clang::SourceLocation location,
    bool expansion)
{
    clang::SourceManager &sm = compiler.getSourceManager();
    if (expansion)
        location = sm.getExpansionLoc(location);
    llvm::StringRef filename = sm.getFilename(location);
    unsigned int number = sm.getSpellingLineNumber(location);

    filename.consume_front("/mnt/build/");
    *_file << filename << ":" << number << ": " << rule.getIdentifier() << '\n';
}

void ReportHandler::reportViolation(coconut::Rule const &rule,
    clang::CompilerInstance &compiler, clang::SourceLocation const &location,
    unsigned int line)
{
    clang::SourceManager &sm = compiler.getSourceManager();
    llvm::StringRef filename = sm.getFilename(location);

    *_file << filename << ":" << line << ": " << rule.getIdentifier() << '\n';
}
