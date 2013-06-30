MESSAGE(STATUS "Setting up chase ...")

aux_source_directory(. CHASE_SRC)
file(GLOB header_files *.hpp)

SET(Boost_USE_STATIC_LIBS        ON)
SET(Boost_USE_MULTITHREADED      ON)


include_directories(
  ${Boost_INCLUDE_DIRS}
  ${CPP-NETLIB_SOURCE_DIR}/uri/src
  ${CPP-NETLIB_SOURCE_DIR}/message/src
  ${CPP-NETLIB_SOURCE_DIR}/logging/src
  ${CPP-NETLIB_SOURCE_DIR}/http/src
  ${CPP-NETLIB_SOURCE_DIR})


ADD_DEFINITIONS (${Boost_DEFINITIONS})
LINK_DIRECTORIES ( ${Boost_LIBRARY_DIRS} )

# enable c++11 for GNU derivates
IF (CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)



add_executable (Chase ${CHASE_SRC})
SET_TARGET_PROPERTIES(Chase PROPERTIES LINKER_LANGUAGE CXX)
TARGET_LINK_LIBRARIES(Chase
  ${Boost_LIBRARIES}
  pthread
  curl
  cppnetlib
  ssl
  crypto
   ${LINKFLAGS})


MESSAGE(STATUS "Done!")



