CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(AirJoy CXX)

#---------------------------------------------------------------------------------------
#
# cxx flags
#
#---------------------------------------------------------------------------------------
IF(WIN32)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ELSE(WIN32)
ENDIF(WIN32)

#---------------------------------------------------------------------------------------
#
# include dir
#
#---------------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

#--------------------------------------------------------------------------------------
#
# lib
#
#--------------------------------------------------------------------------------------
IF (WIN32)

  SET(OS_LIB ws2_32.lib)
    MESSAGE(STATUS "os_lib for Win32: ${OS_LIB}")
 
ELSE (WIN32)

  SET(OS_LIB pthread)
    MESSAGE(STATUS "os_lib for Linux: ${OS_LIB}")

ENDIF (WIN32)

#-------------------------------------------------------------------------------------
# set output dir
#-------------------------------------------------------------------------------------
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)
MESSAGE(STATUS "executable output:  ${EXECUTABLE_OUTPUT_PATH}")
MESSAGE(STATUS "library output: ${LIBRARY_OUTPUT_PATH}")

#-------------------------------------------------------------------------------------
#
# project
#
#-------------------------------------------------------------------------------------
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

