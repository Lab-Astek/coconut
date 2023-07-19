# 🥥 Coconut

The *Code Organisation Checker for Omitting Nasty Uses of Text*, abbreviated Coconut, is an experimental tool to check the students coding style.

It uses Clang's libTooling to analyse code on a semantic level, in order to enforce [Epitech&#39;s C coding style](https://intra.epitech.eu/file/Public/technical-documentations/C/epitech_c_coding_style.pdf).

## Installation

### Using Docker

It is mainly intended to work under Docker, so you can simply use `docker build .` to get the docker image.

Once you have the docker image, you can replace the docker image from [coding-style.sh](https://github.com/Epitech/coding-style-checker) with the one you just built.

You can also replace the docker image from the script to `$(cd /your/build/dir; docker build -q .)` so it automatically updates as you make modifications to coconut.

### Installing locally

If you want to install it locally instead, here's how:

First, make sure you have the dependencies installed. You must have CMake, Bear, and a development version of Clang.

On Fedora, you can run `sudo dnf install bear clang-devel llvm-devel`.

Then, to build locally:

```sh
$ cmake -Bbuild
$ cmake --build build -j
$ sudo cmake --install build
```

The coconut binary will be placed in the /usr/bin directory by the install command. Note that in order to work, the coconut binary **MUST** be in the same directory as the clang binary.

> **Note**: Running `./ftests.sh --local` will run these install steps automatically.

You then just have to invoke the `check.sh` script to run Coconut. You can also put the `check.sh` script in your PATH to make it easier.

## Contributing

If you want to contribute to this repository, please read the guidelines from the [contributing document](CONTRIBUTING.md).

## Supported rules

| Rule identifier | Status                                                   |
|-----------------|----------------------------------------------------------|
| C-O1            | 🍌                                                       |
| C-O2            | ❌ Must be checked by humans                             |
| C-O3            | ✅ Checks for five functions per file, not for coherence |
| C-O4            | ✅ Check snake_case naming, not coherence (file)         |
| C-G1            | 🍌                                                       |
| C-G2            | 🚧                                                       |
| C-G3            | ❌🍌 Planned                                             |
| C-G4            | ✅                                                       |
| C-G5            | ✅                                                       |
| C-G6            | 🍌                                                       |
| C-G7            | 🍌                                                       |
| C-G8            | 🍌                                                       |
| C-G9            | ❌ Planned                                               |
| C-F1            | ❌ Must be checked by humans                             |
| C-F2            | ✅ Checks snake case, not for English or verbs           |
| C-F3            | 🍌                                                       |
| C-F4            | ✅                                                       |
| C-F5            | ✅                                                       |
| C-F6            | ✅                                                       |
| C-F7            | ✅                                                       |
| C-F8            | ✅                                                       |
| C-F9            | ❌ Would not compile as clang doesn't support them       |
| C-L1            | ✅                                                       |
| C-L2            | ❌ Planned                                               |
| C-L3            | ✅                                                       |
| C-L4            | ❌🍌 Planned                                             |
| C-L5            | ✅                                                       |
| C-L6            | ✅                                                       |
| C-V1            | ✅ Not for English                                       |
| C-V2            | ❌ Must be checked by humans                             |
| C-V3            | ✅                                                       |
| C-C1            | ✅                                                       |
| C-C2            | ✅                                                       |
| C-C3            | ✅                                                       |
| C-H1            | ❌ Planned                                               |
| C-H2            | ❌ Planned                                               |
| C-H3            | ✅                                                       |
| C-A1            | ❌ Must be checked by humans                             |
| C-A2            | ❌ Must be checked by humans                             |
| C-A3            | 🍌                                                       |
| C-A4            | ❌ Must be checked by humans                             |

- 🍌 = Handled by Banana
- ✅ = Handled by Coconut
- ❌ = Not supported
