cmake_minimum_required(VERSION 3.0.0)
project(mineserver VERSION 0.1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(mineserver ${SOURCES})
target_include_directories(mineserver PUBLIC src/)

if(CMAKE_BUILD_TYPE EQUAL "Release")
    add_compile_definitions(mineserver PRIVATE -O4)
endif()

# ####################
# OPTIONS
# ####################
option(MINESERVER_ANSI_SUPPORT "Support for ansi color in terminal" ON)

if(MINESERVER_ANSI_SUPPORT)
    target_compile_definitions(mineserver PUBLIC MINESERVER_ANSI_SUPPORT)
endif()

option(MINESERVER_ENABLE_TESTING "Enables the testing" ON)

if(MINESERVER_ENABLE_TESTING)
    add_subdirectory(tests/)
endif()

include(TestBigEndian)
test_big_endian(BIG_ENDIAN)

if(NOT BIG_ENDIAN)
    target_compile_definitions(mineserver PUBLIC __LITTLE_ENDIAN__)
endif()

# ####################
# LIBS
# ####################
add_subdirectory(libs/)
target_link_libraries(mineserver PUBLIC mineserver_libs)