CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3 FATAL_ERROR)

PROJECT(Chase)

set(CMAKE_VERBOSE_MAKEFILE ON)


set(CPP-NETLIB_BUILD_SINGLE_LIB OFF)
set(CPPNETLIB_LIBS
    cppnetlib-uri
    cppnetlib-message
    cppnetlib-message-directives
    cppnetlib-message-wrappers
    cppnetlib-http-message-wrappers
    cppnetlib-http-message
    cppnetlib-constants
    cppnetlib-http-client
    cppnetlib-http-client-connections
#    cppnetlib-logging
)


add_subdirectory(lib/cpp-netlib)
add_subdirectory(src)
add_subdirectory(test)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#   message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
