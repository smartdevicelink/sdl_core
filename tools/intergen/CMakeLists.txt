cmake_minimum_required(VERSION 2.8)
project(intergen)

if (${UNIX})
  add_definitions(-DOS_POSIX)
endif(${UNIX})

add_subdirectory(third_party/pugixml/scripts)
add_subdirectory(utils)
add_subdirectory(model)
add_subdirectory(cppgen)
add_subdirectory(tool)
