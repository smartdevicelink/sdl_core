# Copyright (c) 2018, Ford Motor Company All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

sdl_find_package(boost)

if(TARGET boost::filesystem AND TARGET boost::system AND TARGET boost::thread)
  return()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_SOURCE_PREFIX ${CMAKE_SOURCE_PREFIX} "${3RD_PARTY_INSTALL_PREFIX}")
set(BOOST_LIB_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/boost_src)
set(BOOST_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib)
set_property(GLOBAL PROPERTY GLOBAL_BOOST_LIBS ${BOOST_LIBS_DIRECTORY})
set(BOOST_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include)

include("./${PLATFORM}.cmake")

set(boost_FOUND ON)

file(MAKE_DIRECTORY ${BOOST_INCLUDE_DIRECTORY})

if(NOT TARGET boost::thread)
  add_library(boost::thread UNKNOWN IMPORTED GLOBAL)
  set_target_properties(boost::thread
                        PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                   ${3RD_PARTY_INSTALL_PREFIX}/include
                                   IMPORTED_LOCATION
                                   ${3RD_PARTY_INSTALL_PREFIX}/lib/libboost_thread.so)
  add_dependencies(boost::thread Boost)
endif()

if(NOT TARGET boost::filesystem)
  add_library(boost::filesystem UNKNOWN IMPORTED GLOBAL)
  set_target_properties(boost::filesystem
                        PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                   ${3RD_PARTY_INSTALL_PREFIX}/include
                                   IMPORTED_LOCATION
                                   ${3RD_PARTY_INSTALL_PREFIX}/lib/libboost_filesystem.so)
  add_dependencies(boost::filesystem Boost)
endif()

if(NOT TARGET boost::system)
  add_library(boost::system UNKNOWN IMPORTED GLOBAL)
  set_target_properties(boost::system
                        PROPERTIES IMPORTED_LOCATION
                                   ${3RD_PARTY_INSTALL_PREFIX}/lib/libboost_system.so)
  add_dependencies(boost::system Boost)
endif()
