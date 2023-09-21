
/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** H1
*/

#include "LambdaCallback.hpp"
#include "Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/PreprocessingRecord.h>
#include <clang/Lex/Preprocessor.h>
#include <llvm/ADT/StringRef.h>

using namespace clang::ast_matchers;

coconut::RuleH1::RuleH1()
    : Rule("MAJOR:C-H1", "bad separation between source file and header file")
{
}

template <typename T> static void addSourceFile(MatchFinder &finder, T &handler)
{
    // Trigger for function prototypes in source files
    finder.addMatcher(
        functionDecl(
            isExpansionInMainFile(), unless(isDefinition()),
            unless(isImplicit())
        )
            .bind("decl"),
        &handler
    );
    // Trigger for static inline function definitions in source files
    finder.addMatcher(
        functionDecl(
            isExpansionInMainFile(), isDefinition(), isStaticStorageClass(),
            isInline()
        )
            .bind("decl"),
        &handler
    );
    // Trigger for typedef declarations in source files
    finder.addMatcher(
        typedefDecl(isExpansionInMainFile()).bind("decl"), &handler
    );
    // Trigger for type declarations in source files
    finder.addMatcher(tagDecl(isExpansionInMainFile()).bind("decl"), &handler);
}

static void checkMacrosInSourceFile(
    ReportHandler &report, clang::CompilerInstance &compiler
)
{
    // Trigger for macros in source files
    for (clang::PreprocessedEntity *entity :
         *(compiler.getPreprocessor().getPreprocessingRecord())) {

        clang::MacroDefinitionRecord *macro
            = llvm::dyn_cast<clang::MacroDefinitionRecord>(entity);

        if (macro == nullptr)
            continue;

        clang::SourceManager &sourceManager = compiler.getSourceManager();
        auto loc = macro->getLocation();

        if (not sourceManager.isWrittenInMainFile(loc))
            continue;

        report.reportViolation(coconut::RuleH1(), compiler, loc);
    }
}

template <typename T> static void addHeaderFile(MatchFinder &finder, T &handler)
{
    // Trigger for functions definitions in header files, unless static inline
    finder.addMatcher(
        functionDecl(
            isExpansionInMainFile(), isDefinition(),
            unless(allOf(isStaticStorageClass(), isInline()))
        )
            .bind("decl"),
        &handler
    );
}

void coconut::RuleH1::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    clang::SourceManager &sm = context.getSourceManager();
    auto filename = sm.getFileEntryForID(sm.getMainFileID())->getName();

    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        if (auto stmt = result.Nodes.getNodeAs<clang::Decl>("decl")) {
            report.reportViolation(
                coconut::RuleH1(), compiler, stmt->getBeginLoc()
            );
        }
    });

    if (filename.endswith(".c")) {
        addSourceFile(finder, handler);
        checkMacrosInSourceFile(report, compiler);
    } else if (filename.endswith(".h")) {
        addHeaderFile(finder, handler);
    }
    finder.matchAST(context);
}
