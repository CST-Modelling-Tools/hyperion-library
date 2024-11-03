cmake_minimum_required(VERSION 3.20.2)

# Define the test project
set(This testhypl)
project(${This} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define the sources for the test executable
set(Sources 
    testhypl.cpp
    heliostat-test.cpp
)

# Create the test executable
add_executable(${This} ${Sources})

# Link with Google Test and the main library
target_link_libraries(${This} PRIVATE
    gtest_main
    hyperionlibrary
)

# Include the main library headers
target_include_directories(${This} PRIVATE ../include)

# Enable GoogleTest integration and manually add tests if needed
include(GoogleTest)
gtest_add_tests(TARGET ${This})

# Register the test executable with CTest for running individual tests
add_test(
    NAME ${This}
    COMMAND ${This}
)