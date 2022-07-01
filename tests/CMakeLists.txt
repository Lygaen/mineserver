add_subdirectory(doctest/)
include(CTest)
include_directories(doctest/doctest)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/doctest/scripts/cmake")
include(doctest)

get_filename_component(FULL_PATH_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp ABSOLUTE)
list(REMOVE_ITEM SOURCES "${FULL_PATH_MAIN}")

macro(test name)
    add_executable(${name}-test ${name}.cpp ${SOURCES})
    target_link_libraries(${name}-test doctest mineserver_libs)
    target_include_directories(${name}-test PUBLIC ../src)

    doctest_discover_tests(${name}-test)
endmacro()

test(stream)
test(cryptor)