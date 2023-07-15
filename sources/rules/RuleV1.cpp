/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** V1
*/

#include "Constants.hpp"
#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessingRecord.h>

#include <regex>

#include <iostream> // TODO TMP

using namespace clang::ast_matchers;

coconut::RuleV1::RuleV1()
    : Rule("MINOR:C-V1", "variable name should respect the snake_case convetion")
{
}

void coconut::RuleV1::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;

    LambdaCallback variables([&] (MatchFinder::MatchResult const &result) {
        auto var = result.Nodes.getNodeAs<clang::VarDecl>("variable");

        if (!var)
            return;

        auto loc = var->getLocation();

        if (loc.isMacroID()) {
            // If the variable declaration is in a (system header) macro
            // expansion, it's probably a false positive (ex: FD_ZERO)
            return;
        }

        std::regex regexToCheck(coconut::SNAKECASE_REGEX);

        if (var->hasGlobalStorage())
            return;

        if (not std::regex_match(var->getName().str(), regexToCheck))
            report.reportViolation(*this, compiler, loc);
    });

    LambdaCallback globalVariables([&] (MatchFinder::MatchResult const &result) {
        auto var = result.Nodes.getNodeAs<clang::VarDecl>("global_variable");

        if (!var)
            return;

        auto loc = var->getLocation();

        if (loc.isMacroID()) {
            //// If the variable declaration is in a (system header) macro
            //// expansion, it's probably a false positive (ex: FD_ZERO)
            //return;
        }

        std::regex regexToCheck(coconut::GLOBAL_VAR_SNAKECASE_REGEX);

        if (not std::regex_match(var->getName().str(), regexToCheck))
            report.reportViolation(*this, compiler, loc);
    });

    finder.addMatcher(
        varDecl(
            isExpansionInMainFile()
        ).bind("variable"),
        &variables
    );

    finder.addMatcher(
        varDecl(
            //// we don't want to catch  declared in headers
            isExpansionInMainFile(),
            // also we want to match all global variables
            hasParent(translationUnitDecl()),
            // and then the var need to be const
            hasType(isConstQualified())
        )
            .bind("global_variable"),
        &globalVariables
    );

    for (clang::PreprocessedEntity *entity: *(compiler.getPreprocessor().getPreprocessingRecord())) {

        std::cout << "helo" << std::endl;
        clang::MacroDefinitionRecord *macro = llvm::dyn_cast<clang::MacroDefinitionRecord>(entity);

        if (macro == nullptr)
            continue;

        std::cout << macro->getName()->getName().str() << std::endl;
    }

    finder.matchAST(context);
}
