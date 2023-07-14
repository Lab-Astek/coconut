# Contributing to Coconut

In this document are enumerated the different guidelines related to the contribution to Coconut 🥥.

## Repository structure
### `Dockerfile`
The file describes how the docker image for coconut should be built. It should rarely change.

### `sources`
Directory containing the C++ source files for Coconut.

#### `sources/rules`
Directory containing the rules. Each one represent a rule in the coding style, and is written in C++.

Each rule check if any violations exist, and reports them if necessary.

### `ftests.sh`
This script builds the docker image, and runs all the functional tests.

### `tests`
Directory containing all the test files related to the functional tests for Coconut.

## Adding a rule to Coconut
When adding a rule to Coconut, the following steps should be followed:

1. Create a new branch named `feature/rule-XX` from the `main` branch, with `XX` being the rule's name (without the "C-" part).
2. Create a source file named `sources/rules/RuleXX.cpp`.
3. Add the source file to the `CMakeLists.txt` so it compiles.
4. Add the rule to the `sources/rules/Rules.inc` so the required class is declared and enabled. 
5. Add comprehensive functional tests for the rule.
6. Write the rule (please comment it while doing so, and respect the internal coding style).
7. When all the tests pass (with `./ftests.sh`), open a pull request named `Rule XX: XXXX` (with with `XX` being the rule's identifier, and `XXXX` the comment) aimed at being merged into `main`, and request a review.
8. Iterate on the reviews and requests made.
9. When everything is OK and approved, your PR will be merged into `main`.
10. Congratulations, you have now added a rule to Coconut!
