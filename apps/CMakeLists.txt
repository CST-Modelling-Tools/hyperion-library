cmake_minimum_required(VERSION 3.20.2)

set(This apps)

project(${This} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(
    ../include 
)

add_executable(Main main.cpp)

target_link_libraries(Main PUBLIC
    hyperionlibrary
)

# Tell C++ compiler to use C++20 features. We don't actually use any of them.
#target_compile_features(main PUBLIC cxx_std_20)