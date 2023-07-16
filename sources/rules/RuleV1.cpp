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

using namespace clang::ast_matchers;

coconut::RuleV1::RuleV1()
    : Rule("MINOR:C-V1", "variable name should respect the snake_case convention")
{
}

void coconut::RuleV1::runCheck(ReportHandler &report,
    clang::CompilerInstance &compiler, clang::ASTContext &context) const
{
    MatchFinder finder;

    // First callback that will be call to handle "basic" variables and tags that
    // doesn't respect the snake_case convention, exemple:
    //      enum object {
    //          KNIFE,
    //          HAMMER
    //      };
    //
    // so, we'll check the tag "object"
    LambdaCallback variables([&] (MatchFinder::MatchResult const &result) {
        auto var = result.Nodes.getNodeAs<clang::NamedDecl>("variable");

        if (!var)
            return;

        auto loc = var->getLocation();

        if (loc.isMacroID()) {
            // If the variable declaration is in a (system header) macro
            // expansion, it's probably a false positive (ex: FD_ZERO)
            return;
        }

        std::regex regexToCheck(coconut::SNAKECASE_REGEX);

        if (not std::regex_match(var->getName().str(), regexToCheck))
            report.reportViolation(*this, compiler, loc);
    });


    // Then this callback will be call to handle enum and const global variables that need to be
    // in upper snake_case.
    LambdaCallback enumGlobalVariables([&] (MatchFinder::MatchResult const &result) {
        auto var = result.Nodes.getNodeAs<clang::NamedDecl>("enum_global_variable");

        if (!var)
            return;

        auto loc = var->getLocation();

        std::regex regexToCheck(coconut::GLOBAL_VAR_SNAKECASE_REGEX);

        if (not std::regex_match(var->getName().str(), regexToCheck))
            report.reportViolation(*this, compiler, loc);
    });


    // Here, typedefs will be checked, they need to be in snake_case and to
    // end with _t suffix
    LambdaCallback typedefs([&] (MatchFinder::MatchResult const &result) {
        auto my_typedef = result.Nodes.getNodeAs<clang::TypedefDecl>("typedef");

        if (!my_typedef)
            return;

        std::regex regexToCheck(coconut::TYPEDEF_SNAKECASE_REGEX);

        if (not std::regex_match(my_typedef->getNameAsString(), regexToCheck))
            report.reportViolation(*this, compiler, my_typedef->getLocation());

    });


    // Variables matcher:

    finder.addMatcher(
        varDecl(
            // we don't want to catch variable declared in headers
            isExpansionInMainFile(),
            // also no const global
            unless(
                allOf(
                    hasParent(translationUnitDecl()),
                    hasType(isConstQualified())
                )
            )
        )
            .bind("variable"),
        &variables
    );

    // Const global variables matcher:

    finder.addMatcher(
        varDecl(
            isExpansionInMainFile(),
            // also we want to match all global variables
            hasParent(translationUnitDecl()),
            // and then the var need to be const
            hasType(isConstQualified())
        )
            .bind("enum_global_variable"),
        &enumGlobalVariables
    );

    // Tags declaration matcher:

    finder.addMatcher(
        tagDecl(
            isExpansionInMainFile()
        )
            .bind("variable"),
        &variables
    );

    // Enums enumerators matcher:

    finder.addMatcher(
        enumConstantDecl(
            isExpansionInMainFile()
        )
            .bind("enum_global_variable"),
        &enumGlobalVariables
    );

    // Typedefs matcher:

    finder.addMatcher(
        typedefDecl(
            isExpansionInMainFile()
        )
            .bind("typedef"),
        &typedefs
    );

    // At the end, we'll check that all macros const global are also
    // in maj snake_case

    std::regex regexToCheck(coconut::GLOBAL_VAR_SNAKECASE_REGEX);

    for (clang::PreprocessedEntity *entity: *(compiler.getPreprocessor().getPreprocessingRecord())) {

        clang::MacroDefinitionRecord *macro = llvm::dyn_cast<clang::MacroDefinitionRecord>(entity);

        if (macro == nullptr)
            continue;

        clang::SourceManager &sourceManager = compiler.getSourceManager();
        auto loc = macro->getLocation();

        if (not sourceManager.isWrittenInMainFile(loc))
            continue;

        if (not std::regex_match(macro->getName()->getName().str(), regexToCheck))
            report.reportViolation(*this, compiler, loc);
    }


    finder.matchAST(context);
}
