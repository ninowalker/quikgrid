# cmake build file for QuikGrid
# (c) by Stefan Roettger

PROJECT(QuikGrid)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
OPTION(BUILD_SHARED_LIBS:BOOL "Build as a shared library." ON)

IF (UNIX)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O")
ENDIF (UNIX)

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
IF(BUILD_SHARED_LIBS:BOOL)
	ADD_LIBRARY(quikgrid SHARED ${QUIKGRID_SRCS})
ELSE(BUILD_SHARED_LIBS:BOOL)
	ADD_LIBRARY(quikgrid STATIC ${QUIKGRID_SRCS})
ENDIF(BUILD_SHARED_LIBS:BOOL)


INSTALL(
   TARGETS quikgrid
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   )

INSTALL(
   FILES ${QUIKGRID_HDRS}
   DESTINATION include/quikgrid
   )

