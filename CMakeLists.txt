cmake_minimum_required(VERSION 3.5)
project(mineserver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE MAIN_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_executable(mineserver ${MAIN_SOURCES})
target_include_directories(mineserver PUBLIC src/)

if(MSVC)
  target_compile_options(mineserver PRIVATE /W4 /WX)
else()
  target_compile_options(mineserver PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

option(MINESERVER_ANSI_COLORS "Enables printing with color in the terminal" ON)
if(MINESERVER_ANSI_COLORS)
    target_compile_definitions(mineserver PUBLIC MINESERVER_ANSI_COLORS)
endif()

# LIBRARIES
add_subdirectory(libs/)
target_link_libraries(mineserver PUBLIC mineserver-libs)

# TESTING
option(MINESERVER_BUILD_TESTS "Whether to build or not the tests" ON)
if(MINESERVER_BUILD_TESTS)
    include(CTest)
    add_subdirectory(tests/)
endif()