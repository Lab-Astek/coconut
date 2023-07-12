/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** report handler
*/

#ifndef COCONUT_REPORTHANDLER_HPP
#define COCONUT_REPORTHANDLER_HPP

#include <fstream>
#include <optional>

namespace coconut {
class Rule;
}

namespace clang {
class CompilerInstance;
class SourceLocation;
}

class ReportHandler {
public:
    explicit ReportHandler(std::string const &path);

    void reportViolation(coconut::Rule const &rule,
        clang::CompilerInstance &compiler,
        clang::SourceLocation const &location);
    void report(std::string const &message);

    static std::optional<clang::SourceLocation> getExpansionLoc(
        clang::CompilerInstance &compiler,
        clang::SourceLocation const &location);

private:
    std::ofstream _file;
};

#endif /* !COCONUT_REPORTHANDLER_HPP */
