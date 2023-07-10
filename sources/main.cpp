/*
** EPITECH PROJECT, 2023
** coconut
** File description:
** main
*/

#include "ReportHandler.hpp"

// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

#include <stdexcept>

using namespace clang::tooling;
using namespace llvm;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory coconutCategory("Coconut Options");

static cl::opt<std::string> reportFile(
    "o",
    cl::desc("Specify output report filename"),
    cl::value_desc("filename"),
    cl::cat(coconutCategory),
    cl::Required);

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp commonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp moreHelp("\nChecks Epitech coding style\n");

int main(int argc, char const *argv[])
{
    auto optionsParser = CommonOptionsParser::create(
        argc, argv, coconutCategory);
    if (!optionsParser) {
        // Fail gracefully for unsupported options.
        llvm::errs() << optionsParser.takeError();
        return 1;
    }
    try {
        ReportHandler report(reportFile);
        ClangTool tool(optionsParser->getCompilations(),
                       optionsParser->getSourcePathList());
        return tool.run(
            newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
    } catch (std::runtime_error &e) {
        llvm::errs() << e.what() << "\n";
        return 1;
    }
}
