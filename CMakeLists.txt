CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3 FATAL_ERROR)

PROJECT(Chase)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CPP-NETLIB_BUILD_SINGLE_LIB ON)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib/cpp-netlib)
