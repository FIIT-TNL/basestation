# Find the CUnit includes and library
#
# This module defines
# LIBCONFIGXX_INCLUDE, where to find cppunit include files, etc.
# LIBCONFIGXX_LIBRARIES, the libraries to link against to use CppUnit.
# LIBCONFIGXX_STATIC_LIBRARIY_PATH
# LIBCONFIGXX_FOUND, If false, do not try to use CppUnit.

# also defined, but not for general use are
# LIBCONFIGXX_LIBRARY, where to find the library.

MESSAGE("Searching for libconfig++ library")

if ((NOT LIBCONFIGXX_DIR) AND (DEFINED ENV{LIBCONFIGXX_DIR}))
  file(TO_CMAKE_PATH  "${LIBCONFIGXX_DIR}" LIBCONFIGXX_DIR)
endif()
MESSAGE(STATUS  "LIBCONFIGXX_DIR= ${LIBCONFIGXX_DIR}")
  
FIND_PATH(LIBCONFIGXX_INCLUDE libconfig.h++
  /usr/local/include
  /usr/include
  ~/libs/include
  ~/include
  ${LIBCONFIGXX_DIR}
)

FIND_LIBRARY(LIBCONFIGXX_LIBRARY config++
  /usr/local/lib
  /usr/lib
  ~/libs/lib
  ${LIBCONFIGXX_DIR}
)

FIND_LIBRARY(LIBCONFIGXX_STATIC_LIBRARY "libconfig++${CMAKE_STATIC_LIBRARY_SUFFIX}"
   /usr/local/lib
   /usr/lib
   ~/libs/lib
  ${LIBCONFIGXX_DIR}
)

IF(LIBCONFIGXX_INCLUDE)
  IF(LIBCONFIGXX_LIBRARY)
    SET(LIBCONFIGXX_FOUND TRUE)
    SET(LIBCONFIGXX_LIBRARIES ${LIBCONFIGXX_LIBRARY})
    SET(LIBCONFIGXX_STATIC_LIBRARY_PATH ${LIBCONFIGXX_STATIC_LIBRARY})
  ENDIF(LIBCONFIGXX_LIBRARY)
ENDIF(LIBCONFIGXX_INCLUDE)

IF (LIBCONFIGXX_FOUND)
  IF (NOT LIBCONFIGXX_FIND_QUIETLY)
    MESSAGE(STATUS "Found LibConfig++: ${LIBCONFIGXX_LIBRARIES}")
    MESSAGE(STATUS "static LibConfig++ path: ${LIBCONFIGXX_STATIC_LIBRARY_PATH}")
  ENDIF (NOT LIBCONFIGXX_FIND_QUIETLY)
ELSE (LIBCONFIGXX_FOUND)
  IF (LIBCONFIGXX_FIND_REQUIRED)
    MESSAGE(SEND_ERROR "Could NOT find LibConfig++")
  ENDIF (LIBCONFIGXX_FIND_REQUIRED)
ENDIF (LIBCONFIGXX_FOUND)

# If not found check for LIBCONFIGXX_DIR variable & env. variable
if (NOT LIBCONFIGXX_FOUND)
  set(LIBCONFIGXX_BUILD TRUE)
  if (NOT LIBCONFIGXX_DIR)
    message(WARNING "Libconfig++ not found. Try to set variable or environment variable LIBCONFIGXX_DIR to point to library directory or source directory")
    unset(LIBCONFIGXX_BUILD)
  endif ()
endif (NOT LIBCONFIGXX_FOUND)

  set(LIBCONFIGXX_BUILD TRUE)

# Try to build from source
if (LIBCONFIGXX_BUILD)
  message("Building libconfig++ library from source path ${LIBCONFIGXX_DIR}")

  # Allow the developer to select if Dynamic or Static libraries are built
  option (BUILD_SHARED_LIBS "Build Shared Libraries" ON)
  # Set the LIB_TYPE variable to STATIC
  set (LIB_TYPE STATIC)
  if (BUILD_SHARED_LIBS)
    # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
    set (LIB_TYPE SHARED)
  endif (BUILD_SHARED_LIBS)
  # message(STATUS "libconfig build type: ${LIB_TYPE}")

  set(dir "${LIBCONFIGXX_DIR}")
  file(GLOB LIBCONFIGXX_HEADER_FILES
    "${dir}/*.h"
    "${dir}/*.h++"
    # "${dir}/*.hh"
  )
  
  file(GLOB LIBCONFIGXX_SRC_FILES
    "${dir}/*.c"
    "${dir}/*.c++"
    # "${dir}/*.cc"
  )

  if (MSVC)
    # Prevent Warning C4290: C++ exception specification ignored except to 
    # indicate a function is not __declspec(nothrow)	
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4290")

    # Prevent Warning C4530: C++ exception handler used, but unwind semantics 
    # are not enabled. Specify /EHsc	
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    
    # Prevent Warning C4244: possible loss of data
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244")
    
    # Prevent Warning C4090: 'function' : different 'const' qualifiers
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4090")
    
    # Prevent Warning C4996: 'strcpy': This function or variable may be unsafe...
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
  endif (MSVC)

  # Definitions for DLL vs static 
  if (BUILD_SHARED_LIBS) 
    add_definitions(-DLIBCONFIGXX_EXPORTS -DLIBCONFIG_STATIC)
  else (BUILD_SHARED_LIBS) 
    add_definitions(-DLIBCONFIGXX_STATIC -DLIBCONFIG_STATIC)
  endif (BUILD_SHARED_LIBS) 

  # Create library
  add_library(libconfig++ ${LIB_TYPE} ${LIBCONFIGXX_SRC_FILES} ${LIBCONFIGXX_HEADER_FILES})
  # set_target_properties(libconfig++  PROPERTIES LINKER_LANGUAGE CXX)
  
  set (LIBCONFIGXX_LIBRARIES libconfig++)
  set (LIBCONFIGXX_INCLUDE "${LIBCONFIGXX_DIR}")
  set (LIBCONFIGXX_FOUND TRUE)
  
  if (NOT LIBCONFIGXX_FIND_QUIETLY)
    message(STATUS "LIBCONFIGXX_DIR= ${LIBCONFIGXX_DIR}")
    message(STATUS "LIBCONFIGXX_INCLUDE= ${LIBCONFIGXX_INCLUDE}")
    message(STATUS "LIBCONFIGXX_LIBRARIES= ${LIBCONFIGXX_LIBRARIES}")

    # message(STATUS " -- libconfig src --")
    # foreach( sourcefile ${LIBCONFIGXX_SRC_FILES} )
      # message(STATUS " ${sourcefile}")
    # endforeach(sourcefile)  
    # message(STATUS " -- libconfig headers --")
    # foreach( headerfile ${LIBCONFIGXX_HEADER_FILES} )
      # message(STATUS " ${headerfile}")
    # endforeach(headerfile) 
  endif (NOT LIBCONFIGXX_FIND_QUIETLY)
endif(LIBCONFIGXX_BUILD)

mark_as_advanced(LIBCONFIGXX_INCLUDE LIBCONFIGXX_LIBRARIES)

