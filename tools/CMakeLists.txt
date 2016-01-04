cmake_minimum_required(VERSION 2.8.3)

include(ExternalProject)

set(intergen_SOURCES "${CMAKE_CURRENT_LIST_DIR}/intergen")
set(intergen_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/intergen")
set(intergen_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/intergen")
set(intergen_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${intergen_INSTALL_DIR})

ExternalProject_Add(intergen
                    SOURCE_DIR ${intergen_SOURCES}
                    PREFIX ${intergen_PREFIX}
                    INSTALL_DIR ${intergen_INSTALL_DIR}
                    CMAKE_ARGS ${intergen_CMAKE_ARGS})

# This will force intergen make call every time core is built
# When intergen source didn't change no files are updated
# It is required to automatically rebuild interfaces if intergen was changed
ExternalProject_Add_Step(intergen forcebuild
                         COMMAND ${CMAKE_COMMAND} -E echo "Force intergen build"
                         DEPENDEES update DEPENDERS build
                         ALWAYS 1)

if(BUILD_TESTS)
  add_subdirectory(intergen/test)
endif()  
add_subdirectory(policy_table_validator)
