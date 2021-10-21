cmake_minimum_required(VERSION 3.16)

set(this Http_Server)

project(${this})

set(CMAKE_CXX_STANDARD 11)

string(CONCAT CLANG_TIDY_CHECK_FILTER
    "*,"
    "-fuchsia-*,"
    "-modernize-use-trailing-return-type,"
    "-*-magic-numbers,"
    "-llvm-header-guard,"
    "-hicpp-no-array-decay,"
    "-cppcoreguidelines-pro-bounds-array-to-pointer-decay,"
)

set(CMAKE_CXX_CLANG_TIDY 
    clang-tidy;
    -header-filter=include/;
    -checks=${CLANG_TIDY_CHECK_FILTER};
)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(googletest)

