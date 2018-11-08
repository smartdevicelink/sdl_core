if(TARGET apr-util::apr-util)
  set(apr-util_FOUND ON)
  return()
endif()

find_path(apr-util_INCLUDE_DIR
  NAMES
    apu.h
  PATH_SUFFIXES
    include
)

if(apr-util_INCLUDE_DIR)
  set(apr-util_VERSION_MAJOR 0)
  set(apr-util_VERSION_MINOR 0)
  set(apr-util_VERSION_PATCH 0)

  if(EXISTS "${apr-util_INCLUDE_DIR}/apr-1/apu_version.h")
    # Read and parse apr version header file for version number
    file(STRINGS "${apr-util_INCLUDE_DIR}/apr-1/apu_version.h" _apr_util_version_HEADER_CONTENTS REGEX "#define APU_[A-Z]+_VERSION[ \t]+[0-9]+")
    string(REGEX REPLACE ".*#define APU_MAJOR_VERSION[ \t]+([0-9]+).*" "\\1" apr-util_VERSION_MAJOR "${_apr_util_version_HEADER_CONTENTS}")
    string(REGEX REPLACE ".*#define APU_MINOR_VERSION[ \t]+([0-9]+).*" "\\1" apr-util_VERSION_MINOR "${_apr_util_version_HEADER_CONTENTS}")
    string(REGEX REPLACE ".*#define APU_PATCH_VERSION[ \t]+([0-9]+).*" "\\1" apr-util_VERSION_PATCH "${_apr_util_version_HEADER_CONTENTS}")
    unset(_apr_util_version_HEADER_CONTENTS)
  endif()
  set(apr-util_VERSION_STRING "${apr-util_VERSION_MAJOR}.${apr-util_VERSION_MINOR}.${apr-util_VERSION_PATCH}")
endif ()

find_library(apr-util_LIBRARY
  NAMES
    aprutil-1
  PATH_SUFFIXES
    lib
)

mark_as_advanced(apr-util_INCLUDE_DIR apr-util_LIBRARY apr-util_FOUND)

if(apr-util_LIBRARY AND apr-util_INCLUDE_DIR)
  set(apr-util_FOUND ON)
  message(STATUS "Found apr-util: ${apr-util_LIBRARY} (found version \"${apr-util_VERSION_STRING}\")")
else()
  message(STATUS "apr-util library not found")
  return()
endif()

add_library(apr-util::apr-util UNKNOWN IMPORTED GLOBAL)
set_target_properties(apr-util::apr-util
  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${apr-util_INCLUDE_DIR}"
    IMPORTED_LOCATION "${apr-util_LIBRARY}"
)
