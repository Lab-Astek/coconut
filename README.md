# 🥥 Coconut
The *Code Organisation Checker for Omitting Nasty Uses of Text*, abbreviated Coconut, is an experimental tool to check the students coding style.

It uses Clang's libTooling to analyse code on a semantic level, in order to enforce [Epitech's C coding style](https://intra.epitech.eu/file/Public/technical-documentations/C/epitech_c_coding_style.pdf).

## Installation

It is mainly intended to work under Docker, so you can simply use `docker build .` to get the docker image.

### Installing locally
If you want to install it locally instead, here's how:

```sh
$ cmake -Bbuild
$ cmake --build build -j
$ sudo install build/coconut /usr/bin
```

You then just have to invoke the `check.sh` script to run Coconut. You can also put the `check.sh` script in your PATH to make it easier.

## Contributing
If you want to contribute to this repository, please read the guidelines from the [contributing document](CONTRIBUTING.md).
