/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** V3
*/

#include "LambdaCallback.hpp"
#include "rules/Rules.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTTypeTraits.h>
#include <clang/AST/TypeLoc.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Lexer.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Casting.h>

using namespace clang::ast_matchers;

coconut::RuleV3::RuleV3()
    : Rule("MINOR:C-V3", "pointers must be attached to the associated variable")
{
}

void coconut::RuleV3::runCheck(
    ReportHandler &report, clang::CompilerInstance &compiler,
    clang::ASTContext &context
) const
{
    MatchFinder finder;
    LambdaCallback handler([&](MatchFinder::MatchResult const &result) {
        auto ptr = result.Nodes.getNodeAs<clang::PointerTypeLoc>("ptr");
        if (!ptr)
            return;

        auto star = ptr->getStarLoc();
        if (star.isMacroID()) {
            // Ignore macro expansions
            // NULL would be incorrect as it's `((void*) 0)`
            return;
        }

        clang::SourceManager &sm = compiler.getSourceManager();
        // --- Check spacing between pointer and pointee
        {
            auto pointee = ptr->getPointeeLoc();

            llvm::outs(
            ) << "Line : "
              << sm.getSpellingLineNumber(star) << '\n'
              << "Child "

              << clang::DynTypedNode::create(pointee).getNodeKind().asStringRef(
                 )
              << '\n';
            if (pointee.getAs<clang::ParenTypeLoc>()) {
                // Function pointers or other parenthesized types are not
                // concerned (ex: `int (*ptr)[10];`)
                goto childOK;
            }
            // Unless it's a double pointer, always put a space between the
            // pointer and the pointee
            bool expectSpace = not pointee.getAs<clang::PointerTypeLoc>();
            int hasSpace = sm.getCharacterData(star)[-1] == ' ';
            if (hasSpace != expectSpace) {
                llvm::outs() << "Violation on child "
                             << clang::DynTypedNode::create(pointee)
                                    .getNodeKind()
                                    .asStringRef()
                             << '\n';
                report.reportViolation(*this, compiler, star);
                return;
            }
        }

    childOK:
        auto list = context.getParents(*ptr);
        if (list.empty()) {
            // should not happen
            return;
        }
        auto dynParent = list[0];

        llvm::outs() << "Parent " << dynParent.getNodeKind().asStringRef()
                     << '\n';
        if (dynParent.get<clang::TypeLoc>()
            // Functions returning pointers must not have spaces
            and not dynParent.get<clang::FunctionProtoTypeLoc>()) {
            // It is unspecified whether we should put a space or not:
            // int * const a;
            // int *const a;
            // Both are valid
            // Also, if the parent is a pointer, we've already checked it
            return;
        }
        int hasSpace = sm.getCharacterData(star)[1] == ' ';
        if (hasSpace) {
            llvm::outs() << "Violation on parent "
                         << dynParent.getNodeKind().asStringRef() << '\n';
            report.reportViolation(*this, compiler, star);
        }
    });

    // Match all pointer types
    finder.addMatcher(
        pointerTypeLoc(
            // Only in the main file
            isExpansionInMainFile()
        )
            .bind("ptr"),
        &handler
    );
    finder.matchAST(context);
}
