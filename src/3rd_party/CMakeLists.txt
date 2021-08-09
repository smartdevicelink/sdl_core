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

if(NO_REBUILD_3RD_PARTY)
  set(NO_REBUILD_3RD_PARTY_LOGGER ON)
endif()

if(FORCE_3RD_PARTY)
  if(NO_REBUILD_3RD_PARTY)
    message(FATAL_ERROR "Please don't turn on both FORCE_3RD_PARTY and NO_REBUILD_3RD_PARTY at the same time.")
  else()
    set(FORCE_3RD_PARTY_LOGGER ON)
  endif()
endif()

if(ENABLE_LOG AND NOT ANDROID)
  # --- libexpat
  add_subdirectory(expat-2.1.0)
  set(EXPAT_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX_ARCH}/lib PARENT_SCOPE)

  if(NO_REBUILD_3RD_PARTY_LOGGER)
    message(STATUS "Not rebuilding logger.")
  else()
    set(LOGGER_INSTALL_COMMAND make install)
    if (${USE_DEFAULT_3RD_PARTY_PATH} STREQUAL "true")
      set(LOGGER_INSTALL_COMMAND sudo make install)
    endif()
    if(FORCE_3RD_PARTY_LOGGER)
      message(STATUS "Force to rebuild logger.")

      #build logger
      add_custom_target(3rd_party_logger
        make
        DEPENDS apr-util
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}/apache-log4cxx-cmake
      )

      #install logger
      #install either to default place with sudo or non-default plase without sudo.
      #to install with sudo to non-default place use manual installation
      add_custom_target(install-3rd_party_logger
        COMMAND /bin/bash -c \"USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\;
                               if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then
                                 sudo -k \;
                               fi\;
                               cd ${3RD_PARTY_BINARY_DIRECTORY}/apr-cmake\;
                               ${LOGGER_INSTALL_COMMAND}\;
                               cd ${3RD_PARTY_BINARY_DIRECTORY}/apr-util-cmake\;
                               ${LOGGER_INSTALL_COMMAND}\;
                               cd ${3RD_PARTY_BINARY_DIRECTORY}/apache-log4cxx-cmake\;
                               ${LOGGER_INSTALL_COMMAND}\"
        DEPENDS 3rd_party_logger
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}/apache-log4cxx-cmake
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
                                   cd ${3RD_PARTY_BINARY_DIRECTORY}/apache-log4cxx-cmake\;
                                   make\;
                                 else
                                   echo " Logger is actual. " \;
                                 fi\;
                               else
                                 echo " Need to build logger. " \;
                                 cd ${3RD_PARTY_BINARY_DIRECTORY}/apache-log4cxx-cmake\;
                                 make\;
                               fi\"
        DEPENDS apr-util
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}/apache-log4cxx-build
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
                                     sudo -k \;
                                   fi\;
                                   cd ${3RD_PARTY_BINARY_DIRECTORY}/apr-cmake\;
                                   ${LOGGER_INSTALL_COMMAND}\;
                                   cd ${3RD_PARTY_BINARY_DIRECTORY}/apr-util-cmake\;
                                   ${LOGGER_INSTALL_COMMAND}\;
                                   cd ${3RD_PARTY_BINARY_DIRECTORY}/apache-log4cxx-cmake\;
                                   ${LOGGER_INSTALL_COMMAND}\;
                                 fi\;
                               else
                                 USE_DEFAULT_3RD_PARTY_PATH=${USE_DEFAULT_3RD_PARTY_PATH}\;
                                 if [ \\$$USE_DEFAULT_3RD_PARTY_PATH == "true" ]\; then
                                   sudo -k \;
                                 fi\;
                                 cd ${3RD_PARTY_BINARY_DIRECTORY}/apr-cmake\;
                                 ${LOGGER_INSTALL_COMMAND}\;
                                 cd ${3RD_PARTY_BINARY_DIRECTORY}/apr-util-cmake\;
                                 ${LOGGER_INSTALL_COMMAND}\;
                                 cd ${3RD_PARTY_BINARY_DIRECTORY}/apache-log4cxx-cmake\;
                                 ${LOGGER_INSTALL_COMMAND}\;
                               fi\"
        DEPENDS 3rd_party_logger
        WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}/apache-log4cxx-build
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
  if(NOT ANDROID)
  add_subdirectory(apache-log4cxx-cmake)
  endif()
endif()

if(ANDROID)
  set(OPENSSL_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/openssl_src)
  set(OPENSSL_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include)
  set(OPENSSL_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib)
  set(OPENSSL_FOR_ANDROID_DIR ${3RD_PARTY_SOURCE_DIRECTORY}/android-helpers/openssl)

  include(ExternalProject)
  ExternalProject_Add(
   openssl_external
    URL https://www.openssl.org/source/openssl-1.1.1.tar.gz
    DOWNLOAD_DIR ${OPENSSL_SOURCE_DIRECTORY}
    SOURCE_DIR ${OPENSSL_SOURCE_DIRECTORY}
    CONFIGURE_COMMAND echo "Build openssl for ${ANDROID_PLATFORM}" &&
                      cp ${OPENSSL_FOR_ANDROID_DIR}/patch_openssl.sh ${OPENSSL_SOURCE_DIRECTORY}/patch_openssl.sh &&
                      cd ${OPENSSL_SOURCE_DIRECTORY} &&
                      sh patch_openssl.sh ${ANDROID_PLATFORM}
    BUILD_COMMAND cp ${OPENSSL_FOR_ANDROID_DIR}/build_openssl.sh ${OPENSSL_SOURCE_DIRECTORY}/build_openssl.sh &&
                  sh build_openssl.sh ${CMAKE_ANDROID_NDK} ${ANDROID_PLATFORM} ./ ${3RD_PARTY_INSTALL_PREFIX} ${CMAKE_ANDROID_ARCH_ABI}
    INSTALL_COMMAND ""            
    BUILD_IN_SOURCE true
  )
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${3RD_PARTY_INSTALL_PREFIX}/etc")
set(ENV{PKG_CONFIG_PATH} $ENV{CMAKE_PREFIX_PATH} "${3RD_PARTY_INSTALL_PREFIX}/etc")
find_package(PkgConfig)
pkg_check_modules(BSON libbson)
message (STATUS "bson installed in " ${BSON_LIBDIR} ", " ${BSON_INCLUDEDIR})

if ((NOT "${BSON_FOUND}") OR ("${BSON_VERSION}" VERSION_LESS "1.2.0"))
  message (STATUS "Building bson required")
  set(BSON_LIB_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bson_c_lib CACHE INTERNAL "Sources of bson library" FORCE)
  set(BSON_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib CACHE INTERNAL "Installation path of bson libraries" FORCE)
  set(BSON_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include CACHE INTERNAL "Installation path of bson headers" FORCE)
  set(EMHASHMAP_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib CACHE INTERNAL "Installation path of emashmap libraries" FORCE)
  set(EMHASHMAP_INCLUDE_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/include CACHE INTERNAL "Installation path of emashmap headers" FORCE)

  if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(BSON_FOR_ANDROID_DIR ${3RD_PARTY_SOURCE_DIRECTORY}/android-helpers/bson)
    include(ExternalProject)
    ExternalProject_Add(libbson
            GIT_REPOSITORY "https://github.com/smartdevicelink/bson_c_lib.git"
            GIT_TAG "master"
            DOWNLOAD_DIR ${BSON_LIB_SOURCE_DIRECTORY}
            SOURCE_DIR ${BSON_LIB_SOURCE_DIRECTORY}
            BINARY_DIR ${BSON_LIB_SOURCE_DIRECTORY}
            INSTALL_DIR ${3RD_PARTY_INSTALL_PREFIX}
            CONFIGURE_COMMAND touch aclocal.m4 configure.ac Makefile.am Makefile.in configure config.h.in libbson.pc.in && ./configure --prefix=${3RD_PARTY_INSTALL_PREFIX}
            PATCH_COMMAND ls -1 ${BSON_FOR_ANDROID_DIR} | xargs -L1 -I{} cp ${BSON_FOR_ANDROID_DIR}/{} ${BSON_LIB_SOURCE_DIRECTORY} &&
                          sed -i "s@%ARCH_ABI%@${CMAKE_ANDROID_ARCH_ABI}@" Application.mk
            BUILD_COMMAND cd ${BSON_LIB_SOURCE_DIRECTORY} &&
                          export NDK_PROJECT_PATH=. &&
                          ${CMAKE_ANDROID_NDK}/ndk-build NDK_APPLICATION_MK=Application.mk
            INSTALL_COMMAND make install-data-recursive &&
                            mkdir -p ${3RD_PARTY_INSTALL_PREFIX}/lib &&
                            ls -1 ${BSON_LIB_SOURCE_DIRECTORY}/libs/${CMAKE_ANDROID_ARCH_ABI} | xargs -L1 -I{} cp ${BSON_LIB_SOURCE_DIRECTORY}/libs/${CMAKE_ANDROID_ARCH_ABI}/{} ${3RD_PARTY_INSTALL_PREFIX}/lib
            UPDATE_COMMAND git pull)
  else()
    set(BSON_INSTALL_COMMAND make install)
    if (${3RD_PARTY_INSTALL_PREFIX} MATCHES "/usr/local")
      set(BSON_INSTALL_COMMAND sudo make install)
    endif()
    include(ExternalProject)
    ExternalProject_Add(libbson
            GIT_REPOSITORY "https://github.com/smartdevicelink/bson_c_lib.git"
            GIT_TAG "master"
            BINARY_DIR ${BSON_LIB_SOURCE_DIRECTORY}
            INSTALL_DIR ${3RD_PARTY_INSTALL_PREFIX}
            DOWNLOAD_DIR ${BSON_LIB_SOURCE_DIRECTORY}
            SOURCE_DIR ${BSON_LIB_SOURCE_DIRECTORY}
            CONFIGURE_COMMAND touch aclocal.m4 configure.ac Makefile.am Makefile.in configure config.h.in libbson.pc.in && ./configure --prefix=${3RD_PARTY_INSTALL_PREFIX}
            BUILD_COMMAND make
            INSTALL_COMMAND ${BSON_INSTALL_COMMAND}
            UPDATE_COMMAND git pull)
  endif()

else()
  set(BSON_LIBS_DIRECTORY ${BSON_LIBDIR} CACHE INTERNAL "Installation path of bson libraries" FORCE)
  set(EMHASHMAP_LIBS_DIRECTORY ${BSON_LIBDIR} CACHE INTERNAL "Installation path of emashmap libraries" FORCE)
  add_custom_target(
      libbson
      DEPENDS ${BSON_LIBS_DIRECTORY}
      DEPENDS ${EMHASHMAP_LIBS_DIRECTORY}
      )
endif()

set(BOOST_ROOT ${3RD_PARTY_INSTALL_PREFIX})
find_package(Boost 1.68.0 COMPONENTS system thread date_time filesystem regex)
set(BOOST_LIB_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/boost_src)
set(BOOST_LIBS_DIRECTORY ${3RD_PARTY_INSTALL_PREFIX}/lib)
set(BOOST_INCLUDE_DIR ${3RD_PARTY_INSTALL_PREFIX}/include PARENT_SCOPE)
SET_PROPERTY(GLOBAL PROPERTY GLOBAL_BOOST_LIBS ${BOOST_LIBS_DIRECTORY})
if (NOT ${Boost_FOUND})
  message(STATUS "Did not find boost. Downloading and installing boost 1.68")
  if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(BOOST_FOR_ANDROID_DIR ${3RD_PARTY_SOURCE_DIRECTORY}/android-helpers/boost)
    set(BOOST_FOR_ANDROID_TARGET_VER 21)
    set(BOOST_CLANG_TARGET_ARCH ${CMAKE_ANDROID_ARCH_ABI})
    string(REPLACE "-" "" BOOST_CLANG_TARGET_ARCH ${BOOST_CLANG_TARGET_ARCH})
    string(REPLACE "_" "" BOOST_CLANG_TARGET_ARCH ${BOOST_CLANG_TARGET_ARCH})
    message(STATUS "Traget clang compiler is clang-${BOOST_CLANG_TARGET_ARCH}")

    include(ExternalProject)
    ExternalProject_Add(
      Boost
      URL https://mirror.bazel.build/dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.gz
      URL_HASH SHA256=da3411ea45622579d419bfda66f45cd0f8c32a181d84adfa936f5688388995cf
      DOWNLOAD_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
      SOURCE_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
      CONFIGURE_COMMAND ./bootstrap.sh
      BUILD_COMMAND cp ${BOOST_FOR_ANDROID_DIR}/user-config.jam ${BOOST_LIB_SOURCE_DIRECTORY}/tools/build/src/user-config.jam &&
                    cd ${BOOST_LIB_SOURCE_DIRECTORY}/tools/build/src &&
                    sed -i "s@%NDK_ROOT%@${CMAKE_ANDROID_NDK}@" user-config.jam &&
                    sed -i "s@%BINARIES_PATH%@${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin@" user-config.jam &&
                    sed -i "s@%TARGET_VERSION_32%@${BOOST_FOR_ANDROID_TARGET_VER}@" user-config.jam &&
                    sed -i "s@%TARGET_VERSION_64%@${BOOST_FOR_ANDROID_TARGET_VER}@" user-config.jam &&
                    cd ${BOOST_LIB_SOURCE_DIRECTORY} && patch -p1 < ${BOOST_FOR_ANDROID_DIR}/boost-1_68_0.patch
      INSTALL_COMMAND ./b2 -q -d+2
                      --ignore-site-config -j4
                      target-os=android
                      toolset=clang-${BOOST_CLANG_TARGET_ARCH}
                      link=shared threading=multi
                      --layout=system
                      --build-dir=${BOOST_LIB_SOURCE_DIRECTORY}
                      --prefix=${3RD_PARTY_INSTALL_PREFIX}
                      --with-system --with-thread --with-date_time --with-filesystem --with-regex
                      install > boost_install.log
      INSTALL_DIR ${3RD_PARTY_INSTALL_PREFIX}
      BUILD_IN_SOURCE true
      DEPENDS openssl_external
    )
  else()
    set(BOOST_INSTALL_COMMAND ./b2 install)
    if (${3RD_PARTY_INSTALL_PREFIX} MATCHES "/usr/local")
      set(BOOST_INSTALL_COMMAND sudo ./b2 install)
    endif()
    include(ExternalProject)
    ExternalProject_Add(
      Boost
      URL https://mirror.bazel.build/dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.gz
      URL_HASH SHA256=da3411ea45622579d419bfda66f45cd0f8c32a181d84adfa936f5688388995cf
      DOWNLOAD_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
      SOURCE_DIR ${BOOST_LIB_SOURCE_DIRECTORY}
      CONFIGURE_COMMAND ./bootstrap.sh --with-libraries=system,thread,date_time,filesystem,regex --prefix=${3RD_PARTY_INSTALL_PREFIX}
      BUILD_COMMAND ./b2
      INSTALL_COMMAND ${BOOST_INSTALL_COMMAND} --with-system --with-thread --with-date_time --with-filesystem --with-regex --prefix=${3RD_PARTY_INSTALL_PREFIX} > boost_install.log
      INSTALL_DIR ${3RD_PARTY_INSTALL_PREFIX}
      BUILD_IN_SOURCE true
    )
  endif()
else()
add_custom_target(Boost) # empty target, Boost is already installed
endif()

if(ANDROID)
add_custom_target(install-3rd_party
  DEPENDS libbson
  DEPENDS Boost
  WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
)
else()
add_custom_target(install-3rd_party
  DEPENDS libbson
  DEPENDS Boost
  DEPENDS ${install-3rd_party_logger_var}
  WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
)
endif()