cmake_minimum_required(VERSION 3.5)
project(FortCpp)
enable_language(CXX)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Force the build type")

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Turn off testing" FORCE)
add_subdirectory(benchmark)

add_library(FortCpp INTERFACE)
target_include_directories(FortCpp INTERFACE .)
target_compile_options(FortCpp INTERFACE "--std=c++17")

enable_testing()
add_subdirectory(test)


