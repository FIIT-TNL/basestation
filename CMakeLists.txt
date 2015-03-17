cmake_minimum_required(VERSION 2.8)
project(DriVRBasestation C CXX)

# Distributed CMake Find modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Directories
set(OpenCV_DIR $ENV{OpenCV_DIR})
set(OCULUS_SDK_ROOT_DIR $ENV{OCULUS_SDK_ROOT_DIR})
set(SDL2_DIR $ENV{SDL2_DIR})

set(INCLUDES "")
set(LIBS "")
set(FILES "")

if(WIN32)
  if(CMAKE_CL_64) 
    set(ARCH "x64")
  else()
    set(ARCH "x86")
  endif()
endif()


# SDL2 with quick MARKATALA-style fix to find libs
set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll ${CMAKE_FIND_LIBRARY_SUFFIXES})
set(CMAKE_FIND_ROOT_PATH  ${SDL2_DIR} "${SDL2_DIR}/include")
# find_package(SDL2)
# if(SDL2_INCLUDE_DIR AND WIN32)
  # get_filename_component(SDL2_LIBRARY_TEMP "${SDL2_INCLUDE_DIR}/../lib/${ARCH}/SDL2.lib"  ABSOLUTE)
  # get_filename_component(SDL2_LIBRARY_DIR "${SDL2_INCLUDE_DIR}/../lib/${ARCH}/"  ABSOLUTE)
# endif()
find_package(SDL2 REQUIRED)
set(INCLUDES ${INCLUDES} ${SDL2_INCLUDE_DIR})
set(LIBS ${LIBS} ${SDL2_LIBRARY})

# OpenCV
find_package( OpenCV REQUIRED )
get_filename_component(OpenCV_BIN_PATH "${OpenCV_LIB_PATH}/../bin"  ABSOLUTE)
set(INCLUDES ${INCLUDES} ${OpenCV_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OpenCV_LIBS})

# Oculus SDK
set(CMAKE_FIND_ROOT_PATH "")
find_package( OculusSDK REQUIRED )
get_filename_component(OCULUS_SDK_SRC_DIRS "${OCULUS_SDK_INCLUDE_DIRS}/../Src/"  ABSOLUTE)
set(INCLUDES ${INCLUDES} ${OCULUS_SDK_INCLUDE_DIRS} ${OCULUS_SDK_SRC_DIRS})
set(LIBS ${LIBS} ${OCULUS_SDK_LIBRARIES})
if (WIN32)
  set(FILES ${FILES} stdafx.cpp stdafx.h targetver.h)
endif(WIN32)

message(STATUS "-------------------------------------")
# message(STATUS "CMAKE_CURRENT_SOURCE_DIR=" ${CMAKE_CURRENT_SOURCE_DIR})
# message(STATUS "CMAKE_CURRENT_BINARY_DIR=" ${CMAKE_CURRENT_BINARY_DIR})
# message(STATUS "CMAKE_CL_64 =${CMAKE_CL_64}")
# message(STATUS "CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")
# message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
# message(STATUS "CMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}")
# message(STATUS "CMAKE_FIND_LIBRARY_PREFIXES=${CMAKE_FIND_LIBRARY_PREFIXES}")
# message(STATUS "CMAKE_FIND_LIBRARY_SUFFIXES=${CMAKE_FIND_LIBRARY_SUFFIXES}")
# message(STATUS "--")
message(STATUS "SDL2_DIR=${SDL2_DIR}")
message(STATUS "OpenCV_DIR=${OpenCV_DIR}")
message(STATUS "OCULUS_SDK_ROOT_DIR=${OCULUS_SDK_ROOT_DIR}")
message(STATUS "--")
message(STATUS "SDL2_FOUND     =${SDL2_FOUND}")
message(STATUS "OpenCV_FOUND   =${SDL2_FOUND}")
message(STATUS "OCULUSSDK_FOUND=${OCULUSSDK_FOUND}")
message(STATUS "--")
message(STATUS "SDL2_LIBRARY=${SDL2_LIBRARY}")
message(STATUS "SDL2_INCLUDE_DIR=${SDL2_INCLUDE_DIR}")
message(STATUS "SDL2_LIBRARY_TEMP=${SDL2_LIBRARY_TEMP}")
message(STATUS "SDL2_LIBRARY_DIR=${SDL2_LIBRARY_DIR}")
message(STATUS "OpenCV_BIN_PATH=" ${OpenCV_BIN_PATH})
message(STATUS "OpenCV_LIB_PATH=" ${OpenCV_LIB_PATH})
message(STATUS "OCULUS_SDK_INCLUDE_DIRS=${OCULUS_SDK_INCLUDE_DIRS}")
message(STATUS "OCULUS_SDK_LIBRARIES=${OCULUS_SDK_LIBRARIES}")
message(STATUS "OCULUS_SDK_VERSION=${OCULUS_SDK_VERSION}")
message(STATUS "OCULUS_SDK_SRC_DIRS=${OCULUS_SDK_SRC_DIRS}")
message(STATUS "-------------------------------------")


# Project source & header files
set(FILES oculus.cpp
  ovladanie.h
  ovladanie.cpp
  # texture.hpp
  # texture.cpp
  ${FILES}
)
set(LIBS opengl32
  ${LIBS}
)

message(STATUS "INCLUDES=" ${INCLUDES})
message(STATUS "LIBS=" ${LIBS})
message(STATUS "FILES=" ${FILES})

include_directories(${INCLUDES})
add_executable(Together ${FILES})
target_link_libraries(Together ${LIBS})


# Copy SDL2 DLLs
IF(SDL2_LIBRARY_DIR AND WIN32)
  # Copy DLLs
  message(STATUS "Creating post-install copy commands for SDL2 DLLs")
  file(GLOB_RECURSE sdldllfiles "${SDL2_LIBRARY_DIR}/*.dll")
  foreach( sourcefile ${sdldllfiles} )
    # message(STATUS ${sourcefile})
    add_custom_command(TARGET Together
      POST_BUILD
      COMMAND echo 
        "  ${sourcefile}"
    )
      
    add_custom_command(TARGET Together
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${sourcefile} "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
    )
  endforeach(sourcefile)  
endif()

# Copy OpenCV DLLs
if (WIN32) 
  message(STATUS "Creating post-install copy commands for OpenCV DLLs")
  file(GLOB_RECURSE opencvdllfiles "${OpenCV_BIN_PATH}/*.dll")
  foreach( sourcefile ${opencvdllfiles} )
    # message(STATUS ${sourcefile})
    add_custom_command(TARGET Together
      POST_BUILD
      COMMAND echo 
        "  ${sourcefile}"
    )
      
    add_custom_command(TARGET Together
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${sourcefile} "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
    )
  endforeach(sourcefile)
endif()

# Print all variables  
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
    # message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
