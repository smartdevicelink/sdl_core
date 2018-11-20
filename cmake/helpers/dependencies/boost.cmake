if( TARGET boost::filesystem AND 
    TARGET boost::system AND 
    TARGET boost::thread)
  set(boost_FOUND ON)
  return()
endif()

include("${dependency_descriptor_dir}/boost/filesystem.cmake")
include("${dependency_descriptor_dir}/boost/system.cmake")
include("${dependency_descriptor_dir}/boost/thread.cmake")

if( TARGET boost::filesystem AND 
    TARGET boost::system AND 
    TARGET boost::thread)
  set(boost_FOUND ON)
endif()