/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** rule classes
*/

#ifndef COCONUT_RULES_HPP
#define COCONUT_RULES_HPP

#include "Rule.hpp"

namespace coconut {

#define DECLARE_RULE(name)                                                     \
    class Rule##name : public Rule {                                           \
    public:                                                                    \
        Rule##name();                                                          \
        void runCheck(ReportHandler &report,                                   \
            clang::CompilerInstance &compiler,                                 \
            clang::ASTContext &context) const override;                        \
    };

#include "rules/Rules.inc"

#undef DECLARE_RULE

}

#endif /* !COCONUT_RULES_HPP */
