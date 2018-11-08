if(TARGET boost::system)
    set(boost_system_FOUND ON)
    return()
endif()

find_library(boost_system_LIBRARY
  NAMES
    boost_system
  PATH_SUFFIXES
    lib
)

mark_as_advanced(boost_system_LIBRARY boost_system_FOUND)

if(boost_system_LIBRARY)
  set(boost_system_FOUND ON)
  message(STATUS "boost::system found")
  message(STATUS "boost_system_LIBRARY -> ${boost_system_LIBRARY}")

else()
  message(STATUS "----> boost::system not found!!!")
  return()
endif()

add_library(boost::system UNKNOWN IMPORTED GLOBAL ${boost_system_LIBRARY})
set_target_properties(boost::system
  PROPERTIES
    IMPORTED_LOCATION "${boost_system_LIBRARY}" 
)

