if(TARGET apr::apr)
  set(apr_FOUND ON)
  return()
endif()

find_path(apr_INCLUDE_DIR
  NAMES
    apr.h
  PATH_SUFFIXES
    include
)
set(apr_INCLUDE_DIR "${THIRD_PARTY_INSTALL_PREFIX}/include/apr.h")

if(apr_INCLUDE_DIR)
  set(apr_VERSION_MAJOR 0)
  set(apr_VERSION_MINOR 0)
  set(apr_VERSION_PATCH 0)

  if(EXISTS "${apr_INCLUDE_DIR}/apr-1/apr_version.h")
    # Read and parse apr version header file for version number
    file(STRINGS "${apr_INCLUDE_DIR}/apr-1/apr_version.h" _apr_version_HEADER_CONTENTS REGEX "#define APR_[A-Z]+_VERSION[ \t]+[0-9]+")
    string(REGEX REPLACE ".*#define APR_MAJOR_VERSION[ \t]+([0-9]+).*" "\\1" apr_VERSION_MAJOR "${_apr_version_HEADER_CONTENTS}")
    string(REGEX REPLACE ".*#define APR_MINOR_VERSION[ \t]+([0-9]+).*" "\\1" apr_VERSION_MINOR "${_apr_version_HEADER_CONTENTS}")
    string(REGEX REPLACE ".*#define APR_PATCH_VERSION[ \t]+([0-9]+).*" "\\1" apr_VERSION_PATCH "${_apr_version_HEADER_CONTENTS}")
    unset(_apr_version_HEADER_CONTENTS)
  endif()
  set(apr_VERSION_STRING "${apr_VERSION_MAJOR}.${apr_VERSION_MINOR}.${apr_VERSION_PATCH}")
endif ()

find_library(apr_LIBRARY
  NAMES
    apr-1
  PATH_SUFFIXES
    lib
)
set(apr_LIBRARY "${THIRD_PARTY_INSTALL_PREFIX}/lib/libapr-1.so")
mark_as_advanced(apr_INCLUDE_DIR apr_LIBRARY apr_FOUND)

if(apr_LIBRARY AND apr_INCLUDE_DIR)
  set(apr_FOUND ON)
  message(STATUS "Found apr: ${apr_LIBRARY} (found version \"${apr_VERSION_STRING}\")")
else()
  message(STATUS "apr library not found")
  return()
endif()

add_library(apr::apr UNKNOWN IMPORTED GLOBAL)
set_target_properties(apr::apr
  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${apr_INCLUDE_DIR}"
    IMPORTED_LOCATION "${apr_LIBRARY}"
)
