cmake_minimum_required(VERSION 3.0.0)

set(this HttpServerExe)
project(${this})

# By default, new Visual Studio projects link the C runtimes dynamically 
# but Google Test links them statically. This will generate an error 
# that looks something like the following: gtest.lib(gtest-all.obj) : 
# error LNK2038: mismatch detected for 'RuntimeLibrary': value 'MTd_StaticDebug' 
# doesn't match value 'MDd_DynamicDebug' in main.obj
# Use tis command to force google test to link dynamically
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(CTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(googletest)
add_subdirectory(test)

add_executable(${this} main.cpp)

target_link_libraries(${this} PUBLIC
    HTTP
)
