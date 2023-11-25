project(mineserver-libs)

add_library(mineserver-libs STATIC deps-maker.cpp)

set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_CXX17 ON CACHE BOOL "" FORCE)
add_subdirectory(rapidjson/)

add_subdirectory(zlib/)

add_subdirectory(openssl/)

file(GLOB LUA_SRCS lua/*.c)
add_library(lua ${LUA_SRCS})
target_include_directories(lua PUBLIC lua/)

add_library(luaaa ./luaaa/luaaa.hpp)
set_target_properties(luaaa PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(luaaa PUBLIC luaaa/)
target_link_libraries(luaaa PUBLIC lua)

target_link_libraries(mineserver-libs PUBLIC crypto zlibstatic lua)
target_include_directories(mineserver-libs PUBLIC rapidjson/include zlib)
