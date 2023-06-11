project(mineserver-libs)

add_library(mineserver-libs STATIC deps-maker.cpp)

set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_CXX17 ON CACHE BOOL "" FORCE)
add_subdirectory(rapidjson/)

add_subdirectory(zlib/)

add_subdirectory(openssl/)

target_link_libraries(mineserver-libs PUBLIC crypto zlibstatic)
target_include_directories(mineserver-libs PUBLIC rapidjson/include zlib)
