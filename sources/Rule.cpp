/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** Rules abstract class
*/

#include "Rule.hpp"

Rule::Rule()
{

}

Rule::~Rule()
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
