/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** L5
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Casting.h>

#include <unordered_set>

using namespace clang::ast_matchers;

coconut::RuleL5::RuleL5()
    : Rule(
        "MAJOR:C-L5",
        "variables must be declared at the beginning of the function, one per "
        "line"
    )
{
}

void coconut::RuleL5::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    MatchFinder finder;

    /// The set of variables that were found at the beginning of a function
    std::unordered_set<clang::VarDecl const *> ok_vars;

    /// The set of lines on which a variable was declared
    std::unordered_set<unsigned int> lines_used;

    // This matcher just finds the variables that are declared at the beginning
    // of a function
    LambdaCallback functions([&](MatchFinder::MatchResult const &result) {
        auto stmt = result.Nodes.getNodeAs<clang::FunctionDecl>("function");
        if (!stmt)
            return;

        auto body = llvm::dyn_cast<clang::CompoundStmt>(stmt->getBody());
        if (!body)
            return;

        // Iterate over the initial variable declarations
        // Add them to our set of ok variables
        // The variables not in the set are the ones that are not at the
        // beginning of the function, and they will be catched by the other
        // matcher
        for (auto it = body->body_begin(); it != body->body_end(); ++it) {
            auto decl = llvm::dyn_cast<clang::DeclStmt>(*it);
            if (!decl)
                return;

            for (auto declIt = decl->decl_begin(); declIt != decl->decl_end();
                 ++declIt) {
                auto varDecl = llvm::dyn_cast<clang::VarDecl>(*declIt);
                if (!varDecl)
                    continue; // struct declarations are OK

                ok_vars.insert(varDecl);
            }
        }
    });

    // This is the actual matcher that reports violations on bad variable
    // declarations
    LambdaCallback variables([&](MatchFinder::MatchResult const &result) {
        auto stmt = result.Nodes.getNodeAs<clang::VarDecl>("variable");
        if (!stmt)
            return;

        auto loc = stmt->getLocation();
        auto line = compiler.getSourceManager().getExpansionLineNumber(loc);

        if (loc.isMacroID()) {
            // If the variable declaration is in a (system header) macro
            // expansion, it's probably a false positive (ex: FD_ZERO)
            return;
        }
        if (lines_used.find(line) == lines_used.end()) {
            lines_used.insert(line);
        } else {
            // Multiple variable declarations found on the same line
            report.reportViolation(*this, compiler, loc);
        }
        if (ok_vars.find(stmt) == ok_vars.end()) {
            // Variable declaration not at the beginning of the function
            report.reportViolation(*this, compiler, loc);
        }
    });

    finder.addMatcher(
        functionDecl(
            // we don't want to catch functions declared in headers
            isExpansionInMainFile(),
            // we don't care about function prototypes
            isDefinition()
        )
            .bind("function"),
        &functions
    );

    // Syntactically, a variable declaration can only be in the loop
    // initialisatiion part of a for loop (can't be the condition, that's an
    // expr, and can't be the increment either, and can't be the body (you'd be
    // forced to put it in a compound statement))
    // So we can just check if the parent of the variable declaration is a for
    // loop
    auto inFor = hasParent(declStmt(hasParent(forStmt())));
    finder.addMatcher(
        varDecl(
            // as usual, we don't want to catch variables declared in headers
            isExpansionInMainFile(),
            // we don't care about global variables here
            hasAncestor(functionDecl()),
            // we ignore parameters
            unless(parmVarDecl()),
            // we ignore variables declared in for loops
            unless(inFor)
        )
            .bind("variable"),
        &variables
    );
    finder.matchAST(context);
}
