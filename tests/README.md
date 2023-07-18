# Writing tests

Tests can be written and set up in order to assess the stability and correctness of the rules' analysis.

Currently, only **functional tests** are supported. They check the entire set of rules for violations in each project, and compare with the exected violation report.

These tests can be launched using the `ftests.sh` script at the root of the repository.

By default, the tests will be run under docker. If you want to run them locally instead, use `./ftests --local`. Note that it will need your password for the install step, and may put unwanted files in your repository.

## Functional tests

To create a functional test, create a new directory here, with its name ending in `.t`. By convention, write the directory name in snake case, prefixed with the rule identifier you are especially testing.

Most tests aim at testing one rule at a time.

### `Makefile`

All tests must have a Makefile. You can usually just use the template Makefile you will find below, by just changing the `NAME` variable.

### `expected-reports.log`

This contains the expected content for the coding style report. The content must be exactly the same, in the same order, for the test to pass.

### `coding-style-repors.log`

Once you've run the functional tests, this file will contain the actual report from the last test run. This file must not be pushed, and it is gitignored.

### `.c` files

If you use the template Makefile, your test will only contain a single `NAME.c` file, where `NAME` is the value set for the `NAME` variable inside the Makefile.

You can use another Makefile that compiles multiple files if you need, in which case you will have multiple `.c` files.

### `.h` files

You can have any number of header files in your test, with any name. You can also have none.

If you have a single `.h` file, it is recommended to be `NAME.h` (with `NAME` being the Makefile variable).

## Template Makefile

This is a Makefile you can use for new tests, when you only need a single file to be compiled. You are free to use another makefile for your own tests, or to change it, if it is needed for your test.

You just have to change the value of the `NAME` variable to the name of your test (without a prefix or suffix). For example, the `NAME` for `c2_ternaries.t` is `ternaries`.

```makefile
##
## EPITECH PROJECT, 2023
## coconut test
## File description:
## test
##

NAME	= replace_with_your_test_name

all: $(NAME)

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(NAME)

re: fclean all
```
