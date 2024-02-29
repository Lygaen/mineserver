project(mineserver-libs)

add_library(mineserver-libs STATIC deps-maker.cpp)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_CXX17 ON CACHE BOOL "" FORCE)
add_subdirectory(rapidjson/)

add_subdirectory(zlib/)

# Fixing ZLib shit because we are doing a submodule install, and it is annoying to
# have a dirty git tree just because of one file
file(RENAME ${CMAKE_CURRENT_SOURCE_DIR}/zlib/zconf.h.included ${CMAKE_CURRENT_SOURCE_DIR}/zlib/zconf.h)

add_subdirectory(openssl/)

set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(BUILD_STATIC_CURL ON CACHE BOOL "" FORCE)
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(CURL_ENABLE_EXPORT_TARGET OFF CACHE BOOL "" FORCE)
add_subdirectory(lua-cmake/)

set(LUABRIDGE_TESTING ON CACHE BOOL "" FORCE)
add_subdirectory(LuaBridge3/)

target_link_libraries(mineserver-libs PUBLIC crypto ssl zlibstatic lua::lib LuaBridge)
target_include_directories(mineserver-libs PUBLIC rapidjson/include zlibstatic)
target_compile_options(mineserver-libs PUBLIC -w)
