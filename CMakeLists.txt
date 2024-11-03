cmake_minimum_required(VERSION 3.28.2)

# Project definition
set(This hyperionlibrary)
project(${This} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Enable testing through CTest
include(CTest)
enable_testing()

# Use target-based include directories for modularity
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

# Define source and header files
file(GLOB_RECURSE Headers "${HEADER_DIR}/*.h")
file(GLOB_RECURSE Sources "${SOURCE_DIR}/*.cpp")

# Define the main library target
add_library(${This} SHARED ${Sources} ${Headers})
target_include_directories(${This} PUBLIC ${HEADER_DIR})

# Set DLL export definition for Windows
set_target_properties(${This} PROPERTIES DEFINE_SYMBOL HYPERIONLIBRARY_DLL_EXPORT)

# Google Test setup using FetchContent for isolated dependency management
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz

)
FetchContent_MakeAvailable(googletest)

# Create a test executable
add_subdirectory(test)

# Packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Copy library to the test directory after build
add_custom_command(TARGET hyperionlibrary POST_BUILD 
    COMMAND "${CMAKE_COMMAND}" -E copy 
    "$<TARGET_FILE:hyperionlibrary>"
    "$<TARGET_FILE_DIR:testhypl>/$<TARGET_FILE_NAME:hyperionlibrary>"
    COMMENT "Copying library to test directory"
)