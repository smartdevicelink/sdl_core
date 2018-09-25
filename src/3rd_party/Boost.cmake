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

find_package(Boost 1.66.0 COMPONENTS system thread date_time filesystem) 
set(BOOST_LIB_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/boost_src)
set(BOOST_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib)
SET_PROPERTY(GLOBAL PROPERTY GLOBAL_BOOST_LIBS ${BOOST_LIBS_DIRECTORY})
set(BOOST_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include )

set(
BOOST_QNX_PROJECT_CONFIG_JAM
"using gcc : nto${CMAKE_SYSTEM_PROCESSOR} : ${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-g++${HOST_EXECUTABLE_SUFFIX} : -L${QNX_HOST}/usr/lib -I${QNX_HOST}/usr/include")
set(
  BOOST_GCC_JAM
  sed -Ei "s/case darwin/case *qnx*/g" ./tools/build/src/tools/gcc.jam)
set(
BOOTSTRAP
./bootstrap.sh --with-toolset=gcc --with-libraries=system,thread,date_time,filesystem --prefix=${3RD_PARTY_INSTALL_PREFIX})

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES ".*aarch64")
  set(ADDRESS_MODEL "64")
else()  
  set(ADDRESS_MODEL "32")
endif ()

if (NOT ${Boost_FOUND})
  message(STATUS "Did not find boost. Downloading and installing boost 1.66")
  set(BOOST_INSTALL_COMMAND ./b2 install)
  if (${3RD_PARTY_INSTALL_PREFIX} MATCHES "/usr/local")
    set(BOOST_INSTALL_COMMAND sudo ./b2 install)
  endif()
  include(ExternalProject)  
  ExternalProject_Add(
    Boost
    URL https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
    DOWNLOAD_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
    SOURCE_DIR ${BOOST_LIB_SOURCE_DIRECTORY}  
    CONFIGURE_COMMAND  ${BOOST_GCC_JAM} COMMAND ${BOOTSTRAP}
    BUILD_COMMAND echo ${BOOST_QNX_PROJECT_CONFIG_JAM} $<SEMICOLON> >> ./project-config.jam COMMAND ./b2 address-model=${ADDRESS_MODEL} target-os=qnx toolset=gcc-nto${CMAKE_SYSTEM_PROCESSOR} define=linux     
    INSTALL_COMMAND ${BOOST_INSTALL_COMMAND} --with-system --with-thread --with-date_time --with-filesystem --prefix=${3RD_PARTY_INSTALL_PREFIX} > boost_install.log
    INSTALL_DIR ${3RD_PARTY_INSTALL_PREFIX}
    BUILD_IN_SOURCE true

  )
  set(BOOST_INCLUDE_DIR ${BOOST_ROOT_DIR}/Boost-prefix/src/Boost)
  set(BOOST_LIB_DIR ${BOOST_ROOT_DIR}/Boost-prefix/src/Boost/stage/lib/)   
else()
add_custom_target(Boost) # empty target, Boost is already installed
endif()

