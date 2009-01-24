# cmake build file for QuikGrid
# (c) by Stefan Roettger

PROJECT(QuikGrid)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
OPTION(BUILD_SHARED_LIBS "Build as a shared library." OFF)

IF (UNIX)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O")
ENDIF (UNIX)

#
# Hide advanced variables from the user, so that they won't get confused,
#  i.e. so they don't have to study the documentation to find out whether
#  any of these variables must be set.  They don't.
#
mark_as_advanced(FORCE CMAKE_INSTALL_PREFIX BUILD_SHARED_LIBS CMAKE_BACKWARDS_COMPATIBILITY)
mark_as_advanced(FORCE EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH)

set(CMAKE_SUPPRESS_REGENERATION TRUE)

#
# Default types shown in CMake "Debug;Release;MinSizeRel;RelWithDebInfo",
#  only Debug and Release are useful, so get rid of the junk.
#
IF(CMAKE_CONFIGURATION_TYPES)
  # set(CMAKE_CONFIGURATION_TYPES Debug Release)	# This doesn't work
  # set(CMAKE_CONFIGURATION_TYPES "Debug;Release")	# This doesn't work either
  set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs" FORCE)	# This works!
ENDIF(CMAKE_CONFIGURATION_TYPES)

SET(QUIKGRID_HDRS
  contour.h
  gridxtyp.h
  scatdata.h
  surfgrid.h
  xpand.h
  )

SET(QUIKGRID_SRCS
  contour.cpp
  gridxtyp.cpp
  scatdata.cpp
  surfgrid.cpp
  xpand.cpp
  qgcapi.cpp
  )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# build as a shared library
ADD_LIBRARY(quikgrid ${QUIKGRID_SRCS} ${QUIKGRID_HDRS})

# Distinguish the debug library with a 'D'
set_target_properties(quikgrid PROPERTIES DEBUG_POSTFIX "D")

INSTALL(
   TARGETS quikgrid
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   )

INSTALL(
   FILES ${QUIKGRID_HDRS}
   DESTINATION include/quikgrid
   )

