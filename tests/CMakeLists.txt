enable_testing()
include(CTest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest/)
include(GoogleTest)

macro(test name)
    add_executable(${name}-test ${name}.cpp ${SOURCES})
    target_link_libraries(${name}-test gtest_main mineserver_libs)
    target_include_directories(${name}-test PUBLIC ../src)

    gtest_add_tests(${name}-test SOURCES ${name}.cpp)
endmacro()

test(stream)