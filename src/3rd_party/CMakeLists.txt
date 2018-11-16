# Copyright (c) 2018, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

include("./set_3rd_party_paths.cmake")

set(3RD_PARTY_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(3RD_PARTY_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

if(ENABLE_LOG)
  # --- libexpat
  add_subdirectory(expat-2.1.0)
  set(EXPAT_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)

  set(FORCE_APR_BUILD OFF)
  # --- apr-util
  add_subdirectory(apr-util)

  # --- libapr-1
  add_subdirectory(apr)

  # --- log4cxx
  add_subdirectory(log4cxx)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_SOURCE_PREFIX ${CMAKE_SOURCE_PREFIX} "${3RD_PARTY_INSTALL_PREFIX}")

find_package(Boost 1.66.0 COMPONENTS system thread date_time filesystem)
set(BOOST_LIB_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/boost_src)
set(BOOST_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib)
SET_PROPERTY(GLOBAL PROPERTY GLOBAL_BOOST_LIBS ${BOOST_LIBS_DIRECTORY})
set(BOOST_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include )
if (NOT ${Boost_FOUND})
  message(STATUS "Did not find boost. Downloading and installing boost 1.66")
  if(DEFINED CMAKE_TOOLCHAIN_FILE)
    include("./Boost.cmake")
  else()
     set(BOOST_INSTALL_COMMAND ./b2 install)
     if (${3RD_PARTY_INSTALL_PREFIX} MATCHES "/usr/local")
       set(BOOST_INSTALL_COMMAND sudo ./b2 install)
     endif()
     include(ExternalProject)
     ExternalProject_Add(
       Boost
       URL http://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
       DOWNLOAD_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
       SOURCE_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
       CONFIGURE_COMMAND ./bootstrap.sh --with-libraries=system,thread,date_time,filesystem --prefix=${3RD_PARTY_INSTALL_PREFIX}
       BUILD_COMMAND ./b2
       INSTALL_COMMAND ${BOOST_INSTALL_COMMAND} --with-system --with-thread --with-date_time --with-filesystem --prefix=${3RD_PARTY_INSTALL_PREFIX} > boost_install.log
       INSTALL_DIR ${3RD_PARTY_INSTALL_PREFIX}
       BUILD_IN_SOURCE true
     )
  endif()

set(BOOST_INCLUDE_DIR ${BOOST_ROOT_DIR}/Boost-prefix/src/Boost)
set(BOOST_LIB_DIR ${BOOST_ROOT_DIR}/Boost-prefix/src/Boost/stage/lib/)
else()
  add_custom_target(Boost) # empty target, Boost is already installed
endif()

add_custom_target(3rd_party
  make
  WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
)

add_custom_target(install_3rd_party
  make install
  WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
)
