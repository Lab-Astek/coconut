cmake_minimum_required(VERSION 3.26)

project(coconut)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER g++)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

link_libraries(
    clangAST
    clangBasic
    clangFrontend
    clangLex
    clangTooling
    clangToolingCore
)

set(SOURCES
    sources/main.cpp
)

add_compile_options(-Wall -Wextra)

add_executable(coconut ${SOURCES})
