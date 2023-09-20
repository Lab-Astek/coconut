
/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** H1
*/

#include "LambdaCallback.hpp"
#include "Rules.hpp"
#include "rules/Rules.hpp"

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

static void addSourceFile(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
)
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        if (auto stmt = result.Nodes.getNodeAs<clang::Decl>("func")) {
            report.reportViolation(
                coconut::RuleH1(), compiler, stmt->getBeginLoc()
            );
        }
    });
    // Trigger for function prototypes in source files
    finder.addMatcher(
        functionDecl(
            isExpansionInMainFile(), unless(isDefinition()),
            unless(isImplicit())
        )
            .bind("func"),
        &handler
    );
    // Trigger for static inline function definitions in source files
    finder.addMatcher(
        functionDecl(
            isExpansionInMainFile(), isDefinition(), isStaticStorageClass(),
            isInline()
        )
            .bind("func"),
        &handler
    );
    // Trigger for typedef declarations in source files
    finder.addMatcher(
        typedefDecl(isExpansionInMainFile()).bind("func"),
        &handler
    );
    // Trigger for type declarations in source files
    finder.addMatcher(
        tagDecl(isExpansionInMainFile()).bind("func"),
        &handler
    );
    finder.matchAST(context);

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

static void addHeaderFile(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
)
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        if (auto stmt = result.Nodes.getNodeAs<clang::Decl>("func")) {
            report.reportViolation(
                coconut::RuleH1(), compiler, stmt->getBeginLoc()
            );
        }
    });
    // Trigger for functions definitions in header files, unless static inline
    finder.addMatcher(
        functionDecl(
            isExpansionInMainFile(), isDefinition(),
            unless(allOf(isStaticStorageClass(), isInline()))
        )
            .bind("func"),
        &handler
    );
    finder.matchAST(context);
}

void coconut::RuleH1::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    clang::SourceManager &sm = context.getSourceManager();
    auto filename = sm.getFileEntryForID(sm.getMainFileID())->getName();
    if (filename.endswith(".c")) {
        addSourceFile(report, compiler, context);
    } else if (filename.endswith(".h")) {
        addHeaderFile(report, compiler, context);
    }
}
