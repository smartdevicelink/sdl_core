# Copyright (c) 2014, Ford Motor Company
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

set (install-3rd_party_logger_var "")
set (install-3rd_party_dbus_var "")

if(NO_REBUILD_3RD_PARTY)
  set(NO_REBUILD_3RD_PARTY_LOGGER ON)
  set(NO_REBUILD_3RD_PARTY_DBUS ON)
endif()

if(FORCE_3RD_PARTY)
  if(NO_REBUILD_3RD_PARTY)
  	message(FATAL_ERROR "Please don't turn on both FORCE_3RD_PARTY and NO_REBUILD_3RD_PARTY at the same time.")
  else()
	set(FORCE_3RD_PARTY_LOGGER ON)
	set(FORCE_3RD_PARTY_DBUS ON)
  endif()
endif()

if(ENABLE_LOG OR HMI_DBUS_API)
  # --- libexpat
  add_subdirectory(expat-2.1.0)
  set(EXPAT_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)
endif()

if(ENABLE_LOG)
  if(NO_REBUILD_3RD_PARTY_LOGGER)
    message(STATUS "Not rebuilding logger.")
  else()
    if(FORCE_3RD_PARTY_LOGGER)
      message(STATUS "Force to rebuild logger.")

      #build logger
      add_custom_target(3rd_party_logger
        make
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
      )

      #install logger
      #install either to default place with sudo or non-default plase without sudo.
      #to install with sudo to non-default place use manual installation
      add_custom_target(install-3rd_party_logger
        COMMAND /bin/bash -c \"USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\;
                               if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then
                                 sudo -k \;
                                 sudo make install\;
                               else
                                 make install\;
                               fi\"
        DEPENDS 3rd_party_logger
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
      )
    else()
      #build logger
      add_custom_target(3rd_party_logger
        COMMAND /bin/bash -c \"cd ${CMAKE_CURRENT_SOURCE_DIR} &&
                               grep .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/liblog4cxx.so 1>/dev/null 2>&1\;
                               if [ \\$$? == 0 ]\; then
                                 VAR1=\\$$\( readelf -p .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/liblog4cxx.so 2>/dev/null\)\;
                                 VAR1=\\$$\(echo \\$$VAR1 | awk '{print \\$$NF}'\)\;
                                 VAR2=-1\;
                                 cd ${CMAKE_CURRENT_SOURCE_DIR}\;
                                 git log . 1>/dev/null 2>&1\;
                                 if [ \\$$? == 0 ]; then
                                     VAR2=\\$$\(git log --pretty=\"format:%H\" -1 ${3RD_PARTY_SOURCE_DIRECTORY}/apache-log4cxx-0.10.0\)\;
                                 fi\;
                                 if [ \\$$VAR1 != \\$$VAR2 ]\; then
                                   echo " Need to rebuild logger. " \;
                                   cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                   make\;
                                 else
                                   echo " Logger is actual. " \;
                                 fi\;
                               else
                                 echo " Need to build logger. " \;
                                 cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                 make\;
                               fi\"
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
      )

      #install logger
      #install either to default place with sudo or non-default plase without sudo.
      #to install with sudo to non-default place use manual installation
      add_custom_target(install-3rd_party_logger
        COMMAND /bin/bash -c \"cd ${CMAKE_CURRENT_SOURCE_DIR} &&
                               grep .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/liblog4cxx.so 1>/dev/null 2>&1\;
                               if [ \\$$? == 0 ]\; then
                                 VAR1=\\$$\( readelf -p .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/liblog4cxx.so 2>/dev/null\)\;
                                 VAR1=\\$$\(echo \\$$VAR1 | awk '{print \\$$NF}'\)\;
                                 VAR2=-1\;
                                 cd ${CMAKE_CURRENT_SOURCE_DIR}\;
                                 git log . 1>/dev/null 2>&1\;
                                 if [ \\$$? == 0 ]; then
                                     VAR2=\\$$\(git log --pretty=\"format:%H\" -1 ${3RD_PARTY_SOURCE_DIRECTORY}/apache-log4cxx-0.10.0\)\;
                                 fi\;
                                 if [ \\$$VAR1 != \\$$VAR2 ]\; then
                                   USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\;
                                   if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     sudo -k \;
                                     sudo make install\;
                                   else
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     make install\;
                                   fi\;
                                 fi\;
                               else
                                 USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\;
                                 if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then
                                   cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                   sudo -k \;
                                   sudo make install\;
                                 else
                                   cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                   make install\;
                                 fi\;
                               fi\"
        DEPENDS 3rd_party_logger
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
      )
    endif()

  set(install-3rd_party_logger_var "install-3rd_party_logger")
  endif()

  set(APR_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)
  set(APR_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include PARENT_SCOPE)
  set(APR_UTIL_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)
  set(LOG4CXX_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include PARENT_SCOPE)
  set(LOG4CXX_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)

  # --- libapr-1
  add_subdirectory(apr-cmake)

  # --- apr-util
  add_subdirectory(apr-util-cmake)

  # --- log4cxx
  add_subdirectory(apache-log4cxx-cmake)
endif()

# --- D-Bus
if(HMI_DBUS_API)
  if(NO_REBUILD_3RD_PARTY_DBUS)
    message(STATUS "Not rebuilding D-Bus.")
  else()
    if(FORCE_3RD_PARTY_DBUS)
      message(STATUS "Force to rebuild D-Bus.")

      #build d-bus
      add_custom_target(3rd_party_dbus
        make
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
      )

      #install d-bus
      #install either to default place with sudo or non-default plase without sudo.
      #to install with sudo to non-default place use manual installation
      add_custom_target(install-3rd_party_dbus
        COMMAND /bin/bash -c \"USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\;
                               if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then
                                 sudo -k \;
                                 sudo make install\;
                               else
                                 make install\;
                               fi\"
        DEPENDS 3rd_party_dbus
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
      )
    else()
      #build d-bus
      add_custom_target(3rd_party_dbus
        COMMAND /bin/bash -c \"grep .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/libdbus-1.so 1>/dev/null 2>&1\;
                               if [ \\$$? == 0 ]\; then
                                 VAR1=\\$$\(readelf -p .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/libdbus-1.so 2>/dev/null\)\;
                                 VAR1=\\$$\(echo \\$$VAR1 | awk '{print \\$$NF}'\)\;
                                 VAR2=-1\;
                                 cd ${CMAKE_CURRENT_SOURCE_DIR}\;
                                 git log . 1>/dev/null 2>&1\;
                                 if [ \\$$? == 0 ]; then
                                     VAR2=\\$$\(git log --pretty=\"format:%H\" -1 ${3RD_PARTY_SOURCE_DIRECTORY}/dbus-1.7.8\)\;
                                 fi\;
                                 if [ \\$$VAR1 != \\$$VAR2 ]\; then
                                   echo " Need to rebuild D-Bus. " \;
                                   cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                   make\;
                                 else
                                   echo " D-Bus is actual. " \;
                                 fi\;
                               else
                                 echo " Need to build D-Bus. " \;
                                 cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                 make\;
                               fi\"
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
      )

      #install d-bus
      #install either to default place with sudo or non-default plase without sudo.
      #to install with sudo to non-default place use manual installation
      add_custom_target(install-3rd_party_dbus
        COMMAND /bin/bash -c \"grep .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/libdbus-1.so 1>/dev/null 2>&1\;
                               if [ \\$$? == 0 ]\; then
                                 VAR1=\\$$\(readelf -p .commit_hash ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib/libdbus-1.so 2>/dev/null\)\;
                                 VAR1=\\$$\(echo \\$$VAR1 | awk '{print \\$$NF}'\)\;
                                 VAR2=-1\;
                                 cd ${CMAKE_CURRENT_SOURCE_DIR}\;
                                 git log . 1>/dev/null 2>&1\;
                                 if [ \\$$? == 0 ]; then
                                     VAR2=\\$$\(git log --pretty=\"format:%H\" -1 ${3RD_PARTY_SOURCE_DIRECTORY}/dbus-1.7.8\)\;
                                 fi\;
                                 if [ \\$$VAR1 != \\$$VAR2 ]\; then
                                   USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\;
                                   if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     sudo -k \;
                                     sudo make install\;
                                   else
                                     cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                     make install\;
                                   fi\;
                                 fi\;
                               else
                                 USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\;
                                 if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then
                                   cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                   sudo -k \;
                                   sudo make install\;
                                 else
                                   cd ${3RD_PARTY_BINARY_DIRECTORY}\;
                                   make install\;
                                 fi\;
                               fi\"
        DEPENDS 3rd_party_dbus
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
      )
  
    set(install-3rd_party_dbus_var "install-3rd_party_dbus")
    endif()
  endif()

  # --- D-Bus
  set(DBUS_INCLUDE_DIR ${3RD_PARTY_INSTALL_PREFIX}/include)
  set(DBUS_INCLUDE_DIR_ARCH ${3RD_PARTY_INSTALL_PREFIX_ARCH}/include)
  set(DBUS_INCLUDE_DIRS ${DBUS_INCLUDE_DIR} ${DBUS_INCLUDE_DIR_ARCH})
  set(DBUS_INCLUDE_DIRS ${DBUS_INCLUDE_DIRS} PARENT_SCOPE)
  set(DBUS_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)

  add_subdirectory(dbus-cmake)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_SOURCE_PREFIX ${CMAKE_SOURCE_PREFIX} "${3RD_PARTY_INSTALL_PREFIX}")
find_package (BSON)
message (STATUS "bson installed in " ${BSON_LIBS_DIRECTORY} " , " ${BSON_INCLUDE_DIRECTORY})
message (STATUS "emhashmap installed in " ${EMHASHMAP_LIBS_DIRECTORY} " , " ${EMHASHMAP_INCLUDE_DIRECTORY})

if (${BSON_LIB} MATCHES "BSON_LIB-NOTFOUND")
  message (STATUS "Building bson required")
  set(BSON_LIB_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bson_c_lib CACHE INTERNAL "Sources of bson library" FORCE)
  set(BSON_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib CACHE INTERNAL "Installation path of bson libraries" FORCE)
  set(BSON_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include CACHE INTERNAL "Installation path of bson headers" FORCE)
  set(EMHASHMAP_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib CACHE INTERNAL "Installation path of emashmap libraries" FORCE)
  set(EMHASHMAP_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include CACHE INTERNAL "Installation path of emashmap headers" FORCE)

  set(BSON_INSTALL_COMMAND make install)
  if (${3RD_PARTY_INSTALL_PREFIX} MATCHES "/usr/local")
    set(BSON_INSTALL_COMMAND sudo make install)
  endif()
  include(ExternalProject)
  ExternalProject_Add(libbson
      GIT_REPOSITORY "http://github.com/smartdevicelink/bson_c_lib.git"
      GIT_TAG "master"
      BINARY_DIR ${BSON_LIB_SOURCE_DIRECTORY}
      INSTALL_DIR ${3RD_PARTY_INSTALL_PREFIX}
      DOWNLOAD_DIR ${BSON_LIB_SOURCE_DIRECTORY}
      SOURCE_DIR ${BSON_LIB_SOURCE_DIRECTORY}
      CONFIGURE_COMMAND touch aclocal.m4 configure.ac Makefile.am Makefile.in configure config.h.in && ./configure --prefix=${3RD_PARTY_INSTALL_PREFIX}
      BUILD_COMMAND make
      INSTALL_COMMAND ${BSON_INSTALL_COMMAND})
else()
  get_filename_component(BSON_LIBS_DIRECTORY ${BSON_LIB} DIRECTORY)
  get_filename_component(EMHASHMAP_LIBS_DIRECTORY ${EMHASHMAP_LIB} DIRECTORY)
  set(BSON_LIBS_DIRECTORY ${BSON_LIBS_DIRECTORY} CACHE INTERNAL "Installation path of bson libraries" FORCE)
  set(EMHASHMAP_LIBS_DIRECTORY ${BSON_LIBS_DIRECTORY} CACHE INTERNAL "Installation path of emashmap libraries" FORCE)
endif()

if (HMIADAPTER STREQUAL "messagebroker")
  find_package(Boost 1.66.0 COMPONENTS system) 
  set(BOOST_LIB_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/boost_src)
  set(BOOST_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib)
  SET_PROPERTY(GLOBAL PROPERTY GLOBAL_BOOST_LIBS ${BOOST_LIBS_DIRECTORY})
  set(BOOST_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include )
  if (NOT ${Boost_FOUND})
    message(STATUS "Did not find boost. Downloading and installing boost 1.66")
    include(ExternalProject)
    ExternalProject_Add(
      Boost
      URL https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
      DOWNLOAD_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
      SOURCE_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
      CONFIGURE_COMMAND ./bootstrap.sh --with-libraries=system --prefix=${3RD_PARTY_INSTALL_PREFIX}
      BUILD_COMMAND ./b2
      INSTALL_COMMAND sudo ./b2 install --with-system --prefix=${3RD_PARTY_INSTALL_PREFIX}
      INSTALL_DIR ${3RD_PARTY_INSTALL_PREFIX}
      BUILD_IN_SOURCE true
    )

    set(BOOST_INCLUDE_DIR ${BOOST_ROOT_DIR}/Boost-prefix/src/Boost)
    set(BOOST_LIB_DIR ${BOOST_ROOT_DIR}/Boost-prefix/src/Boost/stage/lib/)
  endif()
endif()

add_custom_target(install-3rd_party
  DEPENDS ${install-3rd_party_logger_var}
  DEPENDS ${install-3rd_party_dbus_var}
  DEPENDS libbson
  DEPENDS Boost
  WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
)

