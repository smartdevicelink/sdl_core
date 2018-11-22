if(boost_FOUND)
  return()
endif()

include("${dependency_descriptor_dir}/boost/filesystem.cmake")
include("${dependency_descriptor_dir}/boost/system.cmake")
include("${dependency_descriptor_dir}/boost/thread.cmake")

if(boost_filesystem_FOUND AND boost_thread_FOUND AND boost_system_FOUND)
  set(boost_FOUND ON)
endif()
