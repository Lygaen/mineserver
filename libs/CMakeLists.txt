project(mineserver-libs)

add_library(mineserver-libs STATIC deps-maker.cpp)

set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_CXX17 ON CACHE BOOL "" FORCE)
add_subdirectory(rapidjson/)

target_include_directories(mineserver-libs PUBLIC rapidjson/include)
