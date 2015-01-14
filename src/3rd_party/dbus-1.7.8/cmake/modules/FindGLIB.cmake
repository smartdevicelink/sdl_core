# - Try to find the GLIB library
# Once done this will define
#
#  GLIB_FOUND - system has GLIB
#  GLIB_INCLUDES - the GLIB include directories
#  GLIB_LIBRARIES - The libraries needed to use GLIB

if (WIN32)

INCLUDE(MacroGetenvWinPath)

MACRO_GETENV_WIN_PATH(_program_FILES_DIR PROGRAMFILES)

FIND_PATH(GLIB_INCLUDE_DIR glib.h
   ${_program_FILES_DIR}/glib/include/glib-2.0
)


# search for GLIB in the default install directory for applications (default of (n)make install)
FIND_LIBRARY(GLIB_LIBRARY NAMES glib-2.0
   PATHS
   ${_program_FILES_DIR}/glib/lib
)

if (GLIB_LIBRARY AND GLIB_INCLUDE_DIR)
   set(GLIB_FOUND TRUE)
   set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${GLIB_INCLUDES})
   set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${GLIB_LIBRARIES})
   
endif (GLIB_LIBRARY AND GLIB_INCLUDE_DIR)

if (GLIB_FOUND)
   if (NOT GLIB_FIND_QUIETLY)
      message(STATUS "Found GLIB: ${GLIB_LIBRARY}")
   endif (NOT GLIB_FIND_QUIETLY)
else (GLIB_FOUND)
   if (GLIB_FIND_REQUIRED)
      message(FATAL_ERROR "Could NOT find GLIB library")
   endif (GLIB_FIND_REQUIRED)
endif (GLIB_FOUND)

endif (WIN32)
