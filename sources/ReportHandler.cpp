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
#include <system_error>

ReportHandler::ReportHandler(std::string const &path)
{
    std::error_code ec;
    _file = std::make_unique<llvm::raw_fd_ostream>(path, ec);
    if (ec)
        throw std::system_error(ec);
}

void ReportHandler::report(std::string const &str)
{
    *_file << str << '\n';
}

void ReportHandler::reportViolation(coconut::Rule const &rule,
    clang::CompilerInstance &compiler, clang::SourceLocation const &location)
{
    clang::SourceManager &sm = compiler.getSourceManager();
    llvm::StringRef filename = sm.getFilename(location);
    unsigned int number = sm.getSpellingLineNumber(location);

    *_file << filename << ":" << number << ": " << rule.getIdentifier()
          << '\n';
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
