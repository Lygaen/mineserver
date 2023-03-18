cmake_minimum_required(VERSION 3.26)
project(mineserver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_executable(mineserver ${SOURCES})
target_include_directories(mineserver PUBLIC src/)

# LIBRARIES
add_subdirectory(libs/)
target_link_libraries(mineserver PUBLIC mineserver-libs)

# TESTING
option(MINESERVER_BUILD_TESTS "Whether to build or not the tests" ON)
if(MINESERVER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/)
endif()