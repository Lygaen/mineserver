set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest/)
enable_testing()
include(GoogleTest)

function(test name)
    add_executable(${name}-test ${name}.cpp)
    target_link_libraries(${name}-test gtest_main mineserver_libs)
    target_include_directories(${name}-test PUBLIC ../src)
    gtest_discover_tests(${name}-test)
endfunction()

test(exemple)
