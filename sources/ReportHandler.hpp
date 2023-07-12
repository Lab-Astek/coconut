/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** report handler
*/

#ifndef COCONUT_REPORTHANDLER_HPP
#define COCONUT_REPORTHANDLER_HPP

#include <optional>
#include <memory>

namespace coconut {
class Rule;
}

namespace clang {
class CompilerInstance;
class SourceLocation;
}

namespace llvm {
class raw_fd_ostream;
}

class ReportHandler {
public:
    explicit ReportHandler(std::string const &path);

    void reportViolation(coconut::Rule const &rule,
        clang::CompilerInstance &compiler,
        clang::SourceLocation const &location);
    void reportViolation(coconut::Rule const &rule,
        clang::CompilerInstance &compiler,
        clang::SourceLocation const &location, int line);

    void report(std::string const &message);

    static std::optional<clang::SourceLocation> getExpansionLoc(
        clang::CompilerInstance &compiler,
        clang::SourceLocation const &location);

private:
    std::unique_ptr<llvm::raw_fd_ostream> _file;
};

#endif /* !COCONUT_REPORTHANDLER_HPP */
