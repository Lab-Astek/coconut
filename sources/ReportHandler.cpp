/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** report handler
*/

#include "ReportHandler.hpp"
#include "Rule.hpp"

#include <clang/Basic/FileManager.h>
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
    unsigned int number = sm.getSpellingLineNumber(location);

    reportViolation(rule, compiler, location, number);
}

void ReportHandler::reportViolation(coconut::Rule const &rule,
    clang::CompilerInstance &compiler, clang::SourceLocation const &location,
    unsigned int line)
{
    clang::SourceManager &sm = compiler.getSourceManager();
    clang::FileManager &fm = sm.getFileManager();
    llvm::StringRef filename = sm.getFilename(location);
    auto entry = fm.getFileRef(filename);

    // We get the absolute path and remove docker's /mnt/build/ prefix
    // so that the path is relative to the project's root, not to the directory
    // of the source's associated Makefile
    if (entry)
        filename = entry.get().getFileEntry().tryGetRealPathName();
    filename.consume_front("/mnt/build/");
    *_file << filename << ":" << line << ": " << rule.getIdentifier() << '\n';
}
