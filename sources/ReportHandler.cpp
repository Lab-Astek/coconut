/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** report handler
*/

#include "ReportHandler.hpp"

ReportHandler::ReportHandler(std::string const &path)
    : _file(path)
{
    if (!_file.is_open())
        throw std::runtime_error("Can't open file");
}

void ReportHandler::report(std::string const &str)
{
    _file << str << std::endl;
}
