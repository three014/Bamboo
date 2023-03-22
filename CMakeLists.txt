cmake_minimum_required(VERSION 3.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_USER_MAKE_RULES_OVERRIDE ./ClangOverrides.txt)

project(RudimentaryNetworkSimulator)

add_subdirectory(aaronscoollibrary)
add_subdirectory(rns)
