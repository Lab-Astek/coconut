/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** report handler
*/

#ifndef COCONUT_REPORTHANDLER_HPP
#define COCONUT_REPORTHANDLER_HPP

#include <fstream>

class ReportHandler {
public:
    explicit ReportHandler(std::string const &path);

    void report(std::string const &message);

private:
    std::ofstream _file;
};

#endif /* !COCONUT_REPORTHANDLER_HPP */
