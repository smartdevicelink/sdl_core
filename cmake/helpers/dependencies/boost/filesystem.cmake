if(TARGET boost::filesystem)
    set(boost_filesystem_FOUND ON)
    return()
endif()

find_path(boost_filesystem_INCLUDE_DIR
  NAMES
    boost/filesystem.hpp
  PATH_SUFFIXES
    include
)

find_library(boost_filesystem_LIBRARY
  NAMES
    boost_filesystem
  PATH_SUFFIXES
    lib
)

mark_as_advanced(boost_filesystem_INCLUDE_DIR boost_filesystem_LIBRARY boost_filesystem_FOUND)

if(boost_filesystem_INCLUDE_DIR AND boost_filesystem_LIBRARY)
  set(boost_filesystem_FOUND ON)
  message(STATUS "boost::filesystem found")
  message(STATUS "boost_filesystem_LIBRARY -> ${boost_filesystem_LIBRARY}")
else()
  message(STATUS "----> boost::filesystem not found!!!")
  return()
endif()

add_library(boost::filesystem UNKNOWN IMPORTED GLOBAL ${boost_system_LIBRARY})
set_target_properties(boost::filesystem
  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${boost_filesystem_INCLUDE_DIR}"
    IMPORTED_LOCATION "${boost_filesystem_LIBRARY}" 
)

