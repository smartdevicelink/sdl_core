SDL_find_package(boost)

if( TARGET boost::filesystem AND 
    TARGET boost::system AND 
    TARGET boost::thread)
  return()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_SOURCE_PREFIX ${CMAKE_SOURCE_PREFIX} "${3RD_PARTY_INSTALL_PREFIX}")
set(BOOST_LIB_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/boost_src)
set(BOOST_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib)
SET_PROPERTY(GLOBAL PROPERTY GLOBAL_BOOST_LIBS ${BOOST_LIBS_DIRECTORY})
set(BOOST_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include )

include("./${PLATFORM}.cmake")

set(boost_FOUND ON)

file(MAKE_DIRECTORY
  ${BOOST_INCLUDE_DIRECTORY}
)

if(NOT TARGET boost::thread)
  add_library(boost::thread UNKNOWN IMPORTED GLOBAL)
  set_target_properties(boost::thread
    PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES ${3RD_PARTY_INSTALL_PREFIX}/include
      IMPORTED_LOCATION  ${3RD_PARTY_INSTALL_PREFIX}/lib/libboost_thread.so
  )
endif()

if(NOT TARGET boost::filesystem)
  add_library(boost::filesystem UNKNOWN IMPORTED GLOBAL)
  set_target_properties(boost::filesystem
    PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES ${3RD_PARTY_INSTALL_PREFIX}/include
      IMPORTED_LOCATION ${3RD_PARTY_INSTALL_PREFIX}/lib/libboost_filesystem.so
  )
endif()

if(NOT TARGET boost::system)
  add_library(boost::system UNKNOWN IMPORTED GLOBAL)
  set_target_properties(boost::system
    PROPERTIES
      IMPORTED_LOCATION ${3RD_PARTY_INSTALL_PREFIX}/lib/libboost_system.so
  )
endif()  
