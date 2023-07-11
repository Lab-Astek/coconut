/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** Rules abstract class
*/

#include "Rule.hpp"

using namespace coconut;

Rule::Rule(std::string identifier, std::string errorDescription)
    : _identifier(identifier)
    , _errorDescription(errorDescription)
{

}
