# DriVR Project
# set DRIVR_QUIET to supress informational output

cmake_minimum_required(VERSION 2.8)
project(DriVR C CXX)

# Distributed CMake Find modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Directories
file(TO_CMAKE_PATH  "$ENV{LIBCONFIGXX_DIR}" LIBCONFIGXX_DIR)
file(TO_CMAKE_PATH  "$ENV{OpenCV_DIR}" OpenCV_DIR)
file(TO_CMAKE_PATH  "$ENV{OCULUS_SDK_ROOT_DIR}" OCULUS_SDK_ROOT_DIR)
file(TO_CMAKE_PATH  "$ENV{SDL2_DIR}" SDL2_DIR)

set(INCLUDES "")
set(LIBS "")
set(FILES "")

# Project source & header files
set(FILES 
  main.cpp
  drivrconfig.hpp
  drivrconfig.cpp
  task.hpp
  task.cpp
  oculus.hpp
  oculus.cpp
  ovladanie.hpp
  ovladanie.cpp
  ${FILES}
)

# LibConfig++
find_package(LibConfig++ REQUIRED)
set(INCLUDES ${INCLUDES} ${LIBCONFIGXX_INCLUDE})
set(LIBS ${LIBS} ${LIBCONFIGXX_LIBRARIES})


# SDL2
find_package(SDL2 REQUIRED)
set(INCLUDES ${INCLUDES} ${SDL2_INCLUDE_DIR})
set(LIBS ${LIBS} ${SDL2_LIBRARY})

# OpenCV
find_package(OpenCV REQUIRED)
get_filename_component(OpenCV_BIN_PATH "${OpenCV_LIB_PATH}/../bin"  ABSOLUTE)
set(INCLUDES ${INCLUDES} ${OpenCV_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OpenCV_LIBS})

# Oculus SDK
set(CMAKE_FIND_ROOT_PATH "")
find_package(OculusSDK REQUIRED)
get_filename_component(OCULUS_SDK_SRC_DIRS "${OCULUS_SDK_INCLUDE_DIRS}/../Src/"  ABSOLUTE)
set(INCLUDES ${INCLUDES} ${OCULUS_SDK_INCLUDE_DIRS} ${OCULUS_SDK_SRC_DIRS})
set(LIBS ${LIBS} ${OCULUS_SDK_LIBRARIES})
if (WIN32)
  set(FILES ${FILES} stdafx.cpp stdafx.h targetver.h)
endif(WIN32)


if (WIN32)
  set(LIBS opengl32 ${LIBS})
endif()

# Executable
include_directories(${INCLUDES})
add_executable(Basestation ${FILES})
target_link_libraries(Basestation ${LIBS})

if (NOT DRIVR_QUIET)
  message("")
  message("=========================================")
  message("> LibConfig++ ---------------------------")
  message(STATUS "LIBCONFIGXX_INCLUDE=${LIBCONFIGXX_INCLUDE}")
  message(STATUS "LIBCONFIGXX_LIBRARIES=${LIBCONFIGXX_LIBRARIES}")
  message("> SDL2 ----------------------------------")
  message(STATUS "SDL2_LIBRARY=${SDL2_LIBRARY}")
  message(STATUS "SDL2_INCLUDE_DIR=${SDL2_INCLUDE_DIR}")
  message(STATUS "SDL2_LIBRARY_TEMP=${SDL2_LIBRARY_TEMP}")
  message(STATUS "SDL2_LIBRARY_DIR=${SDL2_LIBRARY_DIR}")
  message("> OpenCV --------------------------------")
  message(STATUS "OpenCV_BIN_PATH=" ${OpenCV_BIN_PATH})
  message(STATUS "OpenCV_LIB_PATH=" ${OpenCV_LIB_PATH})
  message("> Oculus SDK ----------------------------")
  message(STATUS "OCULUS_SDK_INCLUDE_DIRS=${OCULUS_SDK_INCLUDE_DIRS}")
  message(STATUS "OCULUS_SDK_LIBRARIES=${OCULUS_SDK_LIBRARIES}")
  message(STATUS "OCULUS_SDK_VERSION=${OCULUS_SDK_VERSION}")
  message(STATUS "OCULUS_SDK_SRC_DIRS=${OCULUS_SDK_SRC_DIRS}")
  message("> DIRS ----------------------------------")
  message(STATUS "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
  message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
  message(STATUS "LIBCONFIGXX_DIR=${LIBCONFIGXX_DIR}")
  message(STATUS "SDL2_DIR=${SDL2_DIR}")
  message(STATUS "OpenCV_DIR=${OpenCV_DIR}")
  message(STATUS "OCULUS_SDK_ROOT_DIR=${OCULUS_SDK_ROOT_DIR}")
  message("> DriVR -----------------------------")
  message(STATUS "INCLUDES=" ${INCLUDES})
  message(STATUS "LIBS=" ${LIBS})
  message(STATUS "FILES=" ${FILES})
  message("> FOUND ---------------------------------")
  message(STATUS "LIBCONFIGXX_FOUND =${LIBCONFIGXX_FOUND}")
  message(STATUS "SDL2_FOUND        =${SDL2_FOUND}")
  message(STATUS "OpenCV_FOUND      =${SDL2_FOUND}")
  message(STATUS "OCULUSSDK_FOUND   =${OCULUSSDK_FOUND}")
  message("=========================================")
  message("")
endif (NOT DRIVR_QUIET)


# Copy SDL2 DLLs
IF(SDL2_LIBRARY_DIR AND WIN32)
  # Copy DLLs
  message(STATUS "Creating post-install copy commands for SDL2 DLLs")
  file(GLOB_RECURSE sdldllfiles "${SDL2_LIBRARY_DIR}/*.dll")
  foreach( sourcefile ${sdldllfiles} )
    # message(STATUS ${sourcefile})
    add_custom_command(TARGET Basestation
      POST_BUILD
      COMMAND echo 
        "  ${sourcefile}"
    )
      
    add_custom_command(TARGET Basestation
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${sourcefile} "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
    )
  endforeach(sourcefile)  
endif()

# Copy OpenCV DLLs
if (OpenCV_BIN_PATH AND WIN32) 
  message(STATUS "Creating post-install copy commands for OpenCV DLLs")
  file(GLOB_RECURSE opencvdllfiles "${OpenCV_BIN_PATH}/*.dll")
  foreach( sourcefile ${opencvdllfiles} )
    # message(STATUS ${sourcefile})
    add_custom_command(TARGET Basestation
      POST_BUILD
      COMMAND echo 
        "  ${sourcefile}"
    )
      
    add_custom_command(TARGET Basestation
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${sourcefile} "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
    )
  endforeach(sourcefile)
endif()

# Copy config to binary diresctory
message(STATUS "Copying config file")
if (EXISTS "${CMAKE_BINARY_DIR}/drivr.cfg")
  file(COPY "${CMAKE_BINARY_DIR}/drivr.cfg" DESTINATION "${CMAKE_BINARY_DIR}/drivr.cfg.bkp")
endif()
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/drivr.cfg" DESTINATION "${CMAKE_BINARY_DIR}")

# Print all variables  
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
    # message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()