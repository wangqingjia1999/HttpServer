include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(Socket_Interface_Test
    Socket_Interface_Test.cpp
)
target_link_libraries(Socket_Interface_Test PUBLIC
    Socket_Interface
    gtest_main
)