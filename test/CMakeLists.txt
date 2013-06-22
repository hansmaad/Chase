
MESSAGE(STATUS "Setting up unit testing ...")

aux_source_directory(. UNIT_TEST_SRC)
aux_source_directory(../src CHASE_SRC)
INCLUDE_DIRECTORIES ("${CMAKE_SOURCE_DIR}/src")

SET(Boost_USE_STATIC_LIBS        ON)
SET(Boost_USE_MULTITHREADED      ON)

# Find boost. Uncomment the following line for debug purposes
# SET(Boost_DEBUG TRUE)
FIND_PACKAGE (Boost REQUIRED COMPONENTS unit_test_framework)

INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS (${Boost_DEFINITIONS})
LINK_DIRECTORIES ( ${Boost_LIBRARY_DIRS} )

# enable c++11 for GNU derivates
IF (CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

add_executable (ChaseTest ${UNIT_TEST_SRC} ${CHASE_SRC})
SET_TARGET_PROPERTIES(ChaseTest PROPERTIES LINKER_LANGUAGE CXX)
TARGET_LINK_LIBRARIES(ChaseTest ${Boost_LIBRARIES} pthread ${LINKFLAGS})
ADD_DEPENDENCIES(ChaseTest Chase)

MESSAGE(STATUS "Done!")


MESSAGE(STATUS "Setting up unit test runner ...")

# run test as custom target post_build command
ADD_CUSTOM_TARGET(ChaseTestRunner DEPENDS ChaseTest COMMENT "Running unit tests")
ADD_CUSTOM_COMMAND(TARGET ChaseTestRunner COMMAND $<TARGET_FILE:ChaseTest> VERBATIM)

MESSAGE(STATUS "Done!")
