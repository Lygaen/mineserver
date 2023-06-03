# Add testing library
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest/)
include(GoogleTest)

# Remove main entry from top-executable, to be able to link properly the files
get_filename_component(FULL_PATH_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp ABSOLUTE)
list(REMOVE_ITEM MAIN_SOURCES "${FULL_PATH_MAIN}")

file(GLOB SOURCES CONFIGURE_DEPENDS ./*.cpp)
foreach (FILE ${SOURCES})
    get_filename_component(NAME ${FILE} NAME_WE)
    
    add_executable(${NAME} ${FILE} ${MAIN_SOURCES})
    target_link_libraries(${NAME} PUBLIC gtest_main mineserver-libs)
    target_include_directories(${NAME} PUBLIC ../src/)

    gtest_discover_tests(${NAME})
endforeach ()