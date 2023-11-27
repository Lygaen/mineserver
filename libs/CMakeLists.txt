project(mineserver-libs)

add_library(mineserver-libs STATIC deps-maker.cpp)

set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_CXX17 ON CACHE BOOL "" FORCE)
add_subdirectory(rapidjson/)

add_subdirectory(zlib/)

add_subdirectory(openssl/)

add_subdirectory(lua-cmake/)

set(LUABRIDGE_TESTING ON CACHE BOOL "" FORCE)
add_subdirectory(LuaBridge3/)

target_link_libraries(mineserver-libs PUBLIC crypto zlibstatic lua::lib LuaBridge)
target_include_directories(mineserver-libs PUBLIC rapidjson/include zlib)
target_compile_options(mineserver-libs PUBLIC -w)
