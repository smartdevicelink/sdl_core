cmake_minimum_required(VERSION 2.8)
include(CMakeForceCompiler)
CMAKE_FORCE_CXX_COMPILER(g++ GNU)
CMAKE_FORCE_C_COMPILER(gcc GNU)
project(intergen)

add_definitions(-DOS_POSIX)

add_subdirectory(third_party/pugixml/scripts)
add_subdirectory(utils)
add_subdirectory(model)
add_subdirectory(cppgen)
add_subdirectory(tool)

