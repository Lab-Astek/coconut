cmake_minimum_required(VERSION 3.20)

project(coconut)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER g++)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

add_compile_definitions(LLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING=1)

link_libraries(
    clang-cpp
    LLVM
)

set(SOURCES
    sources/main.cpp
    sources/ReportHandler.cpp
    sources/Rule.cpp
    sources/CoconutStyleAction.cpp

    sources/rules/RuleO3.cpp

    sources/rules/RuleG4.cpp

    sources/rules/RuleF2.cpp
    sources/rules/RuleF4.cpp
    sources/rules/RuleF5.cpp
    sources/rules/RuleF6.cpp
    sources/rules/RuleF7.cpp

    sources/rules/RuleL5.cpp

    sources/rules/RuleC2.cpp
    sources/rules/RuleC3.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/sources
)

add_compile_options(-Wall -Wextra)

add_executable(coconut ${SOURCES})
