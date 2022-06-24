cmake_minimum_required(VERSION 3.0.0)
project(mineserver VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(mineserver ${SOURCES})
target_include_directories(mineserver PUBLIC src/)

# ####################
# OPTIONS
# ####################
option(MINESERVER_ANSI_SUPPORT "Adds support for ansi color in terminal" ON)

if(MINESERVER_ANSI_SUPPORT)
    target_compile_definitions(mineserver PUBLIC MINESERVER_ANSI_SUPPORT)
endif()

# ####################
# LIBS
# ####################
add_subdirectory(libs/)
target_link_libraries(mineserver PUBLIC mineserver_libs)