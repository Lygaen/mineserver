add_library(mineserver_libs deps-maker.cpp)

set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_CXX11 OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_CXX17 ON CACHE BOOL "" FORCE)
add_subdirectory(rapidjson/)
add_subdirectory(libcpnet/)
add_subdirectory(mIni/)
add_subdirectory(fmt/)

target_link_libraries(mineserver_libs PUBLIC mIni fmt)
# because both libraries are header-only and don't include headers at build-time
target_include_directories(mineserver_libs PUBLIC libcpnet/src rapidjson/include)