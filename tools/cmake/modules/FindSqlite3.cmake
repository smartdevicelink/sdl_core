# - Try to find Sqlite3
# Once done this will define
#
#  SQLITE3_FOUND - system has Sqlite3
#  SQLITE3_INCLUDE_DIRS - the Sqlite3 include directory
#  SQLITE3_LIBRARIES - Link these to use Sqlite3
#  SQLITE3_DEFINITIONS - Compiler switches required for using Sqlite3
#
#  Copyright (c) 2008 Andreas Schneider <mail@cynapses.org>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (SQLITE3_LIBRARIES AND SQLITE3_INCLUDE_DIRS)
  set(SQLITE3_FOUND TRUE)
else (SQLITE3_LIBRARIES AND SQLITE3_INCLUDE_DIRS)
  find_package(PkgConfig)
  if (PKG_CONFIG_FOUND)
    pkg_check_modules(_SQLITE3 REQUIRED sqlite3>=3.7.11)
  else (PKG_CONFIG_FOUND)
    message(WARNING "PkgConfig isn't installed. You need to sure sqlite3>=3.7.11")
  endif (PKG_CONFIG_FOUND)

  find_path(SQLITE3_INCLUDE_DIR
    NAMES
      sqlite3.h
    PATHS
      ${_SQLITE3_INCLUDEDIR}
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
  )

  find_library(SQLITE3_LIBRARY
    NAMES
      sqlite3
    PATHS
      ${_SQLITE3_LIBDIR}
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

  if (SQLITE3_LIBRARY)
    set(SQLITE3_FOUND TRUE)
  endif (SQLITE3_LIBRARY)

  set(SQLITE3_INCLUDE_DIRS
    ${SQLITE3_INCLUDE_DIR}
  )

  if (SQLITE3_FOUND)
    set(SQLITE3_LIBRARIES
      ${SQLITE3_LIBRARIES}
      ${SQLITE3_LIBRARY}
    )
  endif (SQLITE3_FOUND)

  if (SQLITE3_INCLUDE_DIRS AND SQLITE3_LIBRARIES)
     set(SQLITE3_FOUND TRUE)
  endif (SQLITE3_INCLUDE_DIRS AND SQLITE3_LIBRARIES)

  if (SQLITE3_FOUND)
    if (NOT Sqlite3_FIND_QUIETLY)
      message(STATUS "Found Sqlite3: ${SQLITE3_LIBRARIES}")
    endif (NOT Sqlite3_FIND_QUIETLY)
  else (SQLITE3_FOUND)
    if (Sqlite3_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Sqlite3")
    endif (Sqlite3_FIND_REQUIRED)
  endif (SQLITE3_FOUND)

  # show the SQLITE3_INCLUDE_DIRS and SQLITE3_LIBRARIES variables only in the advanced view
  mark_as_advanced(SQLITE3_INCLUDE_DIRS SQLITE3_LIBRARIES)

endif (SQLITE3_LIBRARIES AND SQLITE3_INCLUDE_DIRS)