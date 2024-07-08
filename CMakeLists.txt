cmake_minimum_required (VERSION 3.12)

set_property(GLOBAL PROPERTY USE_FOLDERS On)

project(Assignment)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

include(GoogleTest)
set(gtest_force_shared_crt TRUE CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
add_subdirectory(external/googletest)
set_property(TARGET gmock PROPERTY FOLDER external/googletest)
set_property(TARGET gmock_main PROPERTY FOLDER external/googletest)
set_property(TARGET gtest PROPERTY FOLDER external/googletest)
set_property(TARGET gtest_main PROPERTY FOLDER external/googletest)

add_subdirectory(src)
add_subdirectory(test)
