cmake_minimum_required(VERSION 3.20.2)

set(This testhypl)

project(${This} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


include_directories(
    ../include 
)

set(Sources 
    testhypl.cpp 
)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main
    hyperionlibrary
)

add_test(
    NAME ${This}
    COMMAND ${This}
)
