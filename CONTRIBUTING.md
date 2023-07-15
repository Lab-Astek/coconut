# Contributing to Coconut

In this document are enumerated the different guidelines related to the contribution to Coconut 🥥.

## Repository structure
### `Dockerfile`
The file describes how the docker image for coconut should be built. It should rarely change.

### `check.sh`
This is the file that invokes the `coconut` binary on each file to check.
It also generates the necessary compilation database using [Bear](https://github.com/rizsotto/Bear).

It should be run inside of the docker, but supports running outside of it. If it detects it is being run inside docker, it will copy the delivery directory to another directory, to avoid polluting the delivery directory with compilation artifacts.

### `CMakeLists.txt`
We use CMake to build this project. If you don't know how it works, don't worry, you will probably just need to add entries to the SOURCES list.

### `sources`
Directory containing the C++ source files for Coconut.

#### `sources/rules`
Directory containing the rules. Each one represent a rule in the coding style, and is written in C++.

Each rule check if any violations exist, and reports them if necessary.

### `ftests.sh`
This script builds the docker image, and runs all the functional tests.

### `tests`
Directory containing all the test files related to the functional tests for Coconut. Checkout the [test document](tests/README.md) to know how to write tests.

## Adding a rule to Coconut
When adding a rule to Coconut, the following steps should be followed:

1. Create a new branch named `feature/rule-XX` from the `main` branch, with `XX` being the rule's name (without the "C-" part).
2. Create a source file named `sources/rules/RuleXX.cpp`.
3. Add the source file to the `CMakeLists.txt` so it compiles.
4. Add the rule to the `sources/rules/Rules.inc` so the required class is declared and enabled. 
5. Add comprehensive functional [tests](tests/README.md) for the rule.
6. Write the rule (please comment it while doing so, and respect the internal coding style).
7. When all the tests pass (with `./ftests.sh`), open a pull request named `Rule XX: XXXX` (with with `XX` being the rule's identifier, and `XXXX` the comment) aimed at being merged into `main`, and request a review.
8. Iterate on the reviews and requests made.
9. When everything is OK and approved, your PR will be merged into `main`.
10. Congratulations, you have now added a rule to Coconut!

## Useful links about Clang's libTooling

[Introduction to the clang AST (video)](https://clang.llvm.org/docs/IntroductionToTheClangAST.html)

[Introduction to Lib AST Matchers](https://clang.llvm.org/docs/LibASTMatchers.html)

[AST Matchers Reference](https://clang.llvm.org/docs/LibASTMatchersReference.html)

[The coding style of course](https://intra.epitech.eu/file/Public/technical-documentations/C/epitech_c_coding_style.pdf)
