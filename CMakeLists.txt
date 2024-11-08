cmake_minimum_required(VERSION 3.28.2)

# Project definition
set(This hyperionlibrary)
project(${This} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Global output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)

# Make default visibility hidden for all targets
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
enable_testing()

# Use target-based include directories for modularity
set(HEADER_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# Define source and header files
file(GLOB_RECURSE Headers "${HEADER_DIR}/*.h")
file(GLOB_RECURSE Sources "${SOURCE_DIR}/*.cpp")

# Include directories globally for all targets
include_directories(${HEADER_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Define the main library target
add_library(${This} SHARED ${Sources} ${Headers})

# Generate export header
include(GenerateExportHeader)
generate_export_header(${This})

# Google Test setup using the submodule
add_subdirectory(external/googletest)

# Link Google Test libraries to your tests
target_link_libraries(${This} PRIVATE gtest gtest_main)

# Define HYPERIONLIBRARY_EXPORTS when building the shared library
target_compile_definitions(${This} PRIVATE HYPERIONLIBRARY_EXPORTS)

# Search for Python and pybind11
set(PYTHON_WRAPPER_ENABLED FALSE)
find_package(Python COMPONENTS Interpreter Development REQUIRED)

if (Python_FOUND)
    message(STATUS "Found Python interpreter at: ${Python_EXECUTABLE}")

    # Get pybind11 include path via Python script
    execute_process(
        COMMAND ${Python_EXECUTABLE} -c "import pybind11; print(pybind11.get_include())"
        OUTPUT_VARIABLE PYBIND11_INCLUDE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if (PYBIND11_INCLUDE_DIR AND EXISTS "${PYBIND11_INCLUDE_DIR}/pybind11/embed.h")
        message(STATUS "Found pybind11 include directory: ${PYBIND11_INCLUDE_DIR}")
        include_directories(${PYBIND11_INCLUDE_DIR})

        # Get pybind11 CMake directory via Python
        execute_process(
            COMMAND ${Python_EXECUTABLE} -m pybind11 --cmake
            OUTPUT_VARIABLE pybind11_DIR
            ERROR_VARIABLE error
            RESULT_VARIABLE result
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE
        )

        if (result EQUAL 0 AND EXISTS "${pybind11_DIR}/pybind11Config.cmake")
            message(STATUS "Found pybind11 at: ${pybind11_DIR}")
            list(APPEND CMAKE_PREFIX_PATH ${pybind11_DIR})
            find_package(pybind11 CONFIG REQUIRED)

            # Add the Python wrapper module
            pybind11_add_module(${This}_python_wrapper src/wrapper.cpp)
            target_link_libraries(${This}_python_wrapper PRIVATE ${This})
            set(PYTHON_WRAPPER_ENABLED TRUE)
        else()
            message(WARNING "pybind11 not found. Error: ${error}")
        endif()
    else()
        message(WARNING "pybind11 include directory not found.")
    endif()
else()
    message(STATUS "Python not found. Not generating Python wrappers.")
endif()

# Add global compiler flags for Python.h include directory
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /I\"C:/Users/manue_6t240gh/AppData/Local/Programs/Python/Python312/include\"")

# Locate Python library directory and include in link directories
set(PYTHON_LIB_DIR "C:/Users/manue_6t240gh/AppData/Local/Programs/Python/Python312/libs")
if (EXISTS "${PYTHON_LIB_DIR}/python312.lib")
    message(STATUS "Found Python library directory: ${PYTHON_LIB_DIR}")
    target_link_directories(${This} PRIVATE ${PYTHON_LIB_DIR})
    if (PYTHON_WRAPPER_ENABLED)
        target_link_libraries(${This}_python_wrapper PRIVATE ${Python_LIBRARIES} "${PYTHON_LIB_DIR}/python312.lib")
    endif()
else()
    message(WARNING "Python library directory not found or python312.lib missing.")
endif()

# Packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Copy both the Python wrapper module and the main DLL after build
if (PYTHON_WRAPPER_ENABLED)
    set(EXAMPLE_DIR "${CMAKE_SOURCE_DIR}/examples/py_annual_efficiency_matrix")
    
    # Copy the Python wrapper module
    add_custom_command(TARGET ${This}_python_wrapper POST_BUILD 
        COMMAND "${CMAKE_COMMAND}" -E copy 
        "$<TARGET_FILE:${This}_python_wrapper>"
        "${EXAMPLE_DIR}/$<TARGET_FILE_NAME:${This}_python_wrapper>"
        COMMENT "Copying Python wrapper module to example directory"
    )

    # Copy hyperionlibrary.dll to the same directory as the Python wrapper
    add_custom_command(TARGET ${This} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:${This}>"
        "${EXAMPLE_DIR}/$<TARGET_FILE_NAME:${This}>"
        COMMENT "Copying hyperionlibrary.dll to example directory"
    )
endif()

# Add test subdirectory for your tests
add_subdirectory(test)

# Uncomment the following lines to add examples
# add_subdirectory(examples)
# add_subdirectory(examples/py_annual_efficiency_matrix)