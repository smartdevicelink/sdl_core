if(TARGET boost::thread)
  set(boost_thread_FOUND ON)
  return()
endif()

find_path(boost_thread_INCLUDE_DIR NAMES boost/thread.hpp PATH_SUFFIXES include)

find_library(boost_thread_LIBRARY NAMES boost_thread PATH_SUFFIXES lib)

mark_as_advanced(boost_thread_INCLUDE_DIR boost_thread_LIBRARY
                 boost_thread_FOUND)

if(boost_thread_INCLUDE_DIR AND boost_thread_LIBRARY)
  set(boost_thread_FOUND ON)
  message(STATUS "boost::thread found")
  message(STATUS "boost_thread_LIBRARY -> ${boost_thread_LIBRARY}")

else()
  message(STATUS "----> boost::thread not found!!!")
  return()
endif()

add_library(boost::thread UNKNOWN IMPORTED GLOBAL)
set_target_properties(boost::thread
                      PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                 "${boost_thread_INCLUDE_DIR}"
                                 IMPORTED_LOCATION
                                 "${boost_thread_LIBRARY}")
