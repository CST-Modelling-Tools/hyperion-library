cmake_minimum_required(VERSION 3.20.2)

set(This hyperionlibrary)

project(${This} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CTest)
enable_testing()

add_subdirectory(googletest)

include_directories(
    ./include)

set(Headers 
    ./include/atmosphere.h
    ./include/auxfunction.h
    ./include/boundaries.h 
    ./include/environment.h 
    ./include/export.h 
    ./include/gcf.h 
    ./include/heliostat.h 
    ./include/heliostatinstantaneousefficiency.h
    ./include/idealefficiencymap.h 
    ./include/location.h 
    ./include/mathconstants.h 
    ./include/receiver.h 
    ./include/vec2d.h 
    ./include/vec3d.h
)

set(Sources 
    atmosphere.cpp 
    auxfunction.cpp 
    environment.cpp 
    gcf.cpp 
    heliostat.cpp
    heliostatinstantaneousefficiency.cpp
    idealefficiencymap.cpp     
    location.cpp
    vec2d.cpp 
    vec3d.cpp)

add_library(${This} SHARED ${Sources} ${Headers})

add_subdirectory(test)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES DEFINE_SYMBOL pepe)
include(CPack)

add_custom_command(TARGET ${This} POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:${This}>"
     "./test/$<CONFIGURATION>/$<TARGET_FILE_NAME:${This}>" 
  COMMENT "Copying to test directory")
