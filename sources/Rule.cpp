/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** Rules abstract class
*/

#include "Rule.hpp"

Rule::Rule(std::string identifier, std::string errorDescription)
: _identifier(identifier), _errorDescription(errorDescription)
{

}

std::string Rule::getIdentifier() const
{
    return _identifier;
}

std::string Rule::getErrorDescription() const
{
    return _errorDescription;
}
