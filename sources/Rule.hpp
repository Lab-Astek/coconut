/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** Rules abstract class
*/

#ifndef RULE_HPP
#define RULE_HPP

#include <string>

class Rule
{
public:
    Rule(std::string identifier, std::string errorDescription);
    virtual ~Rule() = default;

    std::string getIdentifier() const;
    std::string getErrorDescription() const;

    // virtual void runCheck();

protected:
    std::string _identifier;
    std::string _errorDescription;
};

#endif /* RULE_HPP */
