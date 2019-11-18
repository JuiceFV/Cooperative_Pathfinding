# CMake min version, you can change it for the your own version
cmake_minimum_required(VERSION 3.15)

# The project name
project(Coopirative_Pathfinding)

# detect operating system
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")

# set project version
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}")

message("${PROJECT_NAME} version: ${${PROJECT_NAME}_VERSION}")

# include checkers for the in - source build and build type
include(cmake/safeguard.cmake)
include(cmake/setflags.cmake)

# set directory variable with problem solver sources
set(PS_SRCDIR "sources/Problem_Solver/")

# Set sources variables
set(${PROJECT_NAME}_main "sources/main.cxx")
set(${PROJECT_NAME}_sources "${PS_SRCDIR}Agent.cxx" "${PS_SRCDIR}Map.cxx" "${PS_SRCDIR}Solver.cxx")
set(${PROJECT_NAME}_headers "includes/Agent.hpp" "includes/Map.hpp" "includes/common.hpp" "includes/Solver.hpp")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_main} ${${PROJECT_NAME}_sources} ${${PROJECT_NAME}_headers})
target_include_directories(${PROJECT_NAME} PRIVATE "includes" )
