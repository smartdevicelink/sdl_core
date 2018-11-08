if(TARGET log4cxx::log4cxx)
  set(log4cxx_FOUND ON)
  return()
endif()

find_path(log4cxx_INCLUDE_DIR
  NAMES
    log4cxx/log4cxx.h
  PATH_SUFFIXES
    include
)

set(log4cxx_INCLUDE_DIR "${THIRD_PARTY_INSTALL_PREFIX}/include/log4cxx/")

find_library(log4cxx_LIBRARY
  NAMES
    log4cxx
  PATH_SUFFIXES
  lib
)

set(log4cxx_LIBRARY "${THIRD_PARTY_INSTALL_PREFIX}/lib/liblog4cxx.so")
mark_as_advanced(log4cxx_INCLUDE_DIR log4cxx_LIBRARY log4cxx_FOUND)

if(log4cxx_LIBRARY AND log4cxx_INCLUDE_DIR)
  set(log4cxx_FOUND ON)
  message(STATUS "Found log4cxx: ${log4cxx_LIBRARY}")
else()
  message(STATUS "log4cxx library not found")
  return()
endif()

add_library(log4cxx::log4cxx UNKNOWN IMPORTED GLOBAL)
set_target_properties(log4cxx::log4cxx
  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${log4cxx_INCLUDE_DIR}"
    IMPORTED_LOCATION "${log4cxx_LIBRARY}"
)
