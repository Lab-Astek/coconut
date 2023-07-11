/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** Rules abstract class
*/

#ifndef RULE_HPP
#define RULE_HPP

#include <string>

namespace clang {
class CompilerInstance;
class ASTContext;
}

namespace coconut {

class Rule {
public:
    Rule(std::string identifier, std::string errorDescription);
    virtual ~Rule() = default;

    std::string const &getIdentifier() const
    {
        return _identifier;
    }

    std::string const &getErrorDescription() const
    {
        return _errorDescription;
    }

    virtual void runCheck(clang::CompilerInstance &compiler,
        clang::ASTContext &context) const = 0;

private:
    std::string _identifier;
    std::string _errorDescription;
};

}

#endif /* RULE_HPP */
