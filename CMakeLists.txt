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

cmake_minimum_required(VERSION 3.11)

set(PROJECT smartDeviceLinkCore)
project(${PROJECT})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/tools/cmake/modules/")

include("${CMAKE_CURRENT_LIST_DIR}/cmake/helpers/dependencies.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/tools/cmake/helpers/install_rules.cmake")
include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/platform.cmake)
include(
  "${CMAKE_CURRENT_LIST_DIR}/tools/cmake/helpers/build_config_generator.cmake")

include(CMakeDependentOption)
include(GNUInstallDirs)

if(NOT PLATFORM)
  set(PLATFORM "Linux")
endif()

# Request C++11
set(CMAKE_CXX_STANDARD 11)

# Disable fallback if standard not supported by compiler
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable GNU Extensions
set(CMAKE_CXX_EXTENSIONS ON)

# Enable PIC as static libraries may be linked to shared objects
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

include(
  "${CMAKE_CURRENT_LIST_DIR}/tools/cmake/helpers/git_commit_information.cmake")

# Jenkins integration section dont modify this section if you dont know details
# about integration with Jenkins!!!
set(HMI "web" CACHE STRING "HMI type")
option(
  EXTENDED_MEDIA_MODE
  "Turn on and off extended Madia Manager features relates to PulseAudio A2DP and GStreamer"
  OFF)
option(BUILD_SHARED_LIBS
       "Build all libraries as shared (if ON) or static (if OFF)" OFF)
option(BUILD_BT_SUPPORT "Bluetooth support" ON)
option(BUILD_USB_SUPPORT "libusb support" ON)
option(BUILD_BACKTRACE_SUPPORT "backtrace support" ON)
option(BUILD_TESTS "Possibility to build and run tests" OFF)
option(TELEMETRY_MONITOR "Enable profiling time test util" ON)
option(ENABLE_LOG "Logging feature" ON)
option(ENABLE_GCOV "gcov code coverage feature" OFF)
option(ENABLE_SANITIZE "Sanitize tool" OFF)
option(ENABLE_SECURITY "Security Ford protocol protection" ON)
option(ENABLE_HMI_PTU_DECRYPTION "Policy table update parsed by hmi" ON)
option(USE_COTIRE
       "Use Cotire to speed up build (currently only for commands tests)" ON)
option(USE_GOLD_LD "Use gold linker intead of GNU linker" ON)
option(USE_CCACHE "Turn on ccache usage" ON)
option(USE_DISTCC "Turn on distributed build_usage" OFF)

set(EXTENDED_POLICY "PROPRIETARY"
    CACHE STRING "Policy mode (PROPRIETARY, HTTP or EXTERNAL_PROPRIETARY)")
set_property(CACHE EXTENDED_POLICY
             PROPERTY STRINGS
                      PROPRIETARY
                      HTTP
                      EXTERNAL_PROPRIETARY)
if(EXTENDED_POLICY STREQUAL "")
  set(EXTENDED_POLICY "PROPRIETARY")
endif()

set(OS_TYPE_OPTION "$ENV{OS_TYPE}")
set(HMI_TYPE_OPTION "$ENV{HMI_TYPE}")
set(TARGET_OPTION "$ENV{TARGET}")
set(MEDIA_MODE_OPTION "$ENV{MEDIA_MODE}")
set(HMI_ADAPTER_OPTION "$ENV{HMI_ADAPTER}")
set(ENABLE_TESTS_OPTION "$ENV{ENABLE_TESTS}")
set(ENABLE_LOG_OPTION "$ENV{ENABLE_LOG}")
set(ARCH_TYPE_OPTION "$ENV{ARCH_TYPE}")
set(POLICY_OPTION "$ENV{POLICY_TYPE}")
set(SECURITY_OPTION "$ENV{SECURITY_MODE}")
set(COMPONENTS_DIR ${CMAKE_SOURCE_DIR}/src/components)
set(SNAPSHOT_TAG "$ENV{SNAPSHOT_TAG}")
set(CMAKE_CXX_COMPILER $ENV{CMAKE_CXX_COMPILER})
set(CMAKE_C_COMPILER $ENV{CMAKE_C_COMPILER})
set(BUILDDIR "$ENV{BUILDDIR}")

if(ARCH_TYPE_OPTION)
  if(NOT
     (${ARCH_TYPE_OPTION} STREQUAL "x86")
     AND NOT (${ARCH_TYPE_OPTION} STREQUAL "armv7"))
    message(
      AUTHOR_WARNING
        "HW architecture is not defined, using x86. Allowed values are x86/armv7 (case sensitive)"
      )
    set(ARCH_TYPE_OPTION "x86")
  endif()
else()
  set(ARCH_TYPE_OPTION "x86")
endif()

if(HMI_TYPE_OPTION)
  if(${HMI_TYPE_OPTION} STREQUAL "NONE")
    message(STATUS "Jenkins integration: select HMI none")
    set(HMI "no")
  else()
    message(STATUS "Jenkins integration: select HTML5 HMI")
    set(HMI "web")
  endif()
endif()

if(MEDIA_MODE_OPTION)
  if(${MEDIA_MODE_OPTION} STREQUAL "EXTENDED_MEDIA")
    message(STATUS "Jenkins integration: select extended media mode")
    set(EXTENDED_MEDIA_MODE ON)
  endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(
    CMAKE_BUILD_TYPE Debug
    CACHE
      STRING
      "Choose the type of build. Options are: None, Debug, Release, RelWithDebInfo, MinSizeRel."
    FORCE)
endif()

if(HMI_ADAPTER_OPTION)
  if(${HMI_ADAPTER_OPTION} STREQUAL "MESSAGEBROKER")
    message(STATUS "Jenkins integration: selected HMI adapter MESSAGEBROKER")
    set(HMIADAPTER "messagebroker")
  endif()
endif()

if(ENABLE_LOG_OPTION)
  if(${ENABLE_LOG_OPTION} STREQUAL "LOG_OFF")
    message(STATUS "Jenkins integration: Log is turned off")
    set(ENABLE_LOG OFF)
  endif()
endif()

if(ENABLE_TESTS_OPTION)
  if(${ENABLE_TESTS_OPTION} STREQUAL "TESTS_OFF")
    message(STATUS "Jenkins integration: Unit tests is turned off")
    set(BUILD_TESTS OFF)
  elseif(${ENABLE_TESTS_OPTION} STREQUAL "TESTS_ON")
    message(STATUS "Jenkins integration: Unit tests is turned on")
    set(BUILD_TESTS ON)
  endif()
endif()

if(SECURITY_OPTION)
  if(${SECURITY_OPTION} STREQUAL "SEC_OFF")
    message(STATUS "Jenkins integration: Security is turned OFF")
    set(ENABLE_SECURITY OFF)
  endif()
endif()

set(objcopy "objcopy")
if(OS_TYPE_OPTION)
  if(${OS_TYPE_OPTION} STREQUAL "QNX")
    message(STATUS "Jenkins integration: set build process for QNX")
    # do not use include after project() command. Such usecase results in
    # infinite cycle of reinitialization of compiler and other variables
    include("./qnx_6.5.0_linux_x86.cmake")
    set(objcopy "nto${ARCH_TYPE_OPTION}-objcopy")
    # tests are not supported yet for QNX build
    set(BUILD_TESTS OFF)
  endif()
endif()

# Jenkins integration section end

# Deliver Build Configuration

set(build_config_path "src/appMain/build_config.txt")
file(WRITE "${build_config_path}" "")

get_property(cHelpString CACHE EXTENDED_POLICY PROPERTY HELPSTRING)
get_property(cType CACHE EXTENDED_POLICY PROPERTY TYPE)
get_property(cValue CACHE EXTENDED_POLICY PROPERTY VALUE)
file(APPEND "${build_config_path}" "//${cHelpString}\n")
file(APPEND "${build_config_path}" "EXTENDED_POLICY:${cType}=${cValue}\n\n")

get_property(cHelpString CACHE BUILD_BT_SUPPORT PROPERTY HELPSTRING)
get_property(cType CACHE BUILD_BT_SUPPORT PROPERTY TYPE)
get_property(cValue CACHE BUILD_BT_SUPPORT PROPERTY VALUE)
file(APPEND "${build_config_path}" "//${cHelpString}\n")
file(APPEND "${build_config_path}" "BUILD_BT_SUPPORT:${cType}=${cValue}\n\n")

get_property(cHelpString CACHE BUILD_USB_SUPPORT PROPERTY HELPSTRING)
get_property(cType CACHE BUILD_USB_SUPPORT PROPERTY TYPE)
get_property(cValue CACHE BUILD_USB_SUPPORT PROPERTY VALUE)
file(APPEND "${build_config_path}" "//${cHelpString}\n")
file(APPEND "${build_config_path}" "BUILD_USB_SUPPORT:${cType}=${cValue}\n\n")

get_property(cHelpString CACHE ENABLE_SECURITY PROPERTY HELPSTRING)
get_property(cType CACHE ENABLE_SECURITY PROPERTY TYPE)
get_property(cValue CACHE ENABLE_SECURITY PROPERTY VALUE)
file(APPEND "${build_config_path}" "//${cHelpString}\n")
file(APPEND "${build_config_path}" "ENABLE_SECURITY:${cType}=${cValue}\n\n")

get_property(cHelpString CACHE EXTENDED_MEDIA_MODE PROPERTY HELPSTRING)
get_property(cType CACHE EXTENDED_MEDIA_MODE PROPERTY TYPE)
get_property(cValue CACHE EXTENDED_MEDIA_MODE PROPERTY VALUE)
file(APPEND "${build_config_path}" "//${cHelpString}\n")
file(APPEND "${build_config_path}" "EXTENDED_MEDIA_MODE:${cType}=${cValue}\n\n")

get_property(cHelpString CACHE TELEMETRY_MONITOR PROPERTY HELPSTRING)
get_property(cType CACHE TELEMETRY_MONITOR PROPERTY TYPE)
get_property(cValue CACHE TELEMETRY_MONITOR PROPERTY VALUE)
file(APPEND "${build_config_path}" "//${cHelpString}\n")
file(APPEND "${build_config_path}" "TELEMETRY_MONITOR:${cType}=${cValue}\n\n")

get_property(cHelpString CACHE HMI PROPERTY HELPSTRING)
get_property(cType CACHE HMI PROPERTY TYPE)
get_property(cValue CACHE HMI PROPERTY VALUE)
file(APPEND "${build_config_path}" "//${cHelpString}\n")
file(APPEND "${build_config_path}" "HMI:${cType}=${cValue}\n\n")

get_property(cHelpString CACHE ENABLE_HMI_PTU_DECRYPTION PROPERTY HELPSTRING)
get_property(cType CACHE ENABLE_HMI_PTU_DECRYPTION PROPERTY TYPE)
get_property(cValue CACHE ENABLE_HMI_PTU_DECRYPTION PROPERTY VALUE)
file(APPEND "${build_config_path}" "//${cHelpString}\n")
file(APPEND "${build_config_path}"
            "ENABLE_HMI_PTU_DECRYPTION:${cType}=${cValue}\n")

add_custom_target(
  pasa-tarball
  COMMAND ${CMAKE_SOURCE_DIR}/tools/Utils/export-customer-specific.sh
          ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} pasa
  COMMAND tar -cz -C /tmp/PASA -f ${CMAKE_BINARY_DIR}/pasa.tar.gz .
  DEPENDS HMI_API MOBILE_API v4_protocol_v1_2_no_extra)

add_custom_target(
  ford-tarball
  COMMAND ${CMAKE_SOURCE_DIR}/tools/Utils/export-customer-specific.sh
          ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ford
  COMMAND tar -cz -C /tmp/FORD -f ${CMAKE_BINARY_DIR}/ford.tar.gz .
  DEPENDS HMI_API MOBILE_API v4_protocol_v1_2_no_extra)

add_custom_target(
  genivi-tarball
  COMMAND ${CMAKE_SOURCE_DIR}/tools/Utils/export-customer-specific.sh
          ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} genivi
  COMMAND tar -cz -C /tmp/GENIVI -f ${CMAKE_BINARY_DIR}/genivi.tar.gz .)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Please do not change compiler/linker flags if You do not know how particular
# flag is handled by CMake
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

set(CMAKE_CXX_FLAGS "-fPIC -Wall -Werror -Wuninitialized -Wvla")

if(USE_CCACHE)
  # Configure CCache if available
  find_program(CCACHE_FOUND ccache)
  if(CCACHE_FOUND)
    get_property(RULE_LAUNCH_COMPILE GLOBAL PROPERTY RULE_LAUNCH_COMPILE)
    set(RULE_LAUNCH_COMPILE "${RULE_LAUNCH_COMPILE} ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${RULE_LAUNCH_COMPILE})

    get_property(RULE_LAUNCH_LINK GLOBAL PROPERTY RULE_LAUNCH_LINK)
    set(RULE_LAUNCH_LINK "${RULE_LAUNCH_LINK} ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${RULE_LAUNCH_LINK})

    message(INFO " Used CCache for compilation.")
  else(CCACHE_FOUND)
    message(WARNING " CCache was not found.")
  endif(CCACHE_FOUND)
endif(USE_CCACHE)

if(USE_DISTCC)
  # Configure distributed compilation if available
  find_program(DISTCC_FOUND distcc)
  if(DISTCC_FOUND)
    get_property(RULE_LAUNCH_COMPILE GLOBAL PROPERTY RULE_LAUNCH_COMPILE)
    set(RULE_LAUNCH_COMPILE "${RULE_LAUNCH_COMPILE} distcc")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${RULE_LAUNCH_COMPILE})
    message(INFO " Used distributed build.")
  else(CCACHE_FOUND)
    message(
      WARNING " distcc was not found. Distributed compilation is impossible.")
  endif(DISTCC_FOUND)
endif(USE_DISTCC)

if(ENABLE_SANITIZE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  message(STATUS "Sanitizing enabled")
endif()

if(ENABLE_GCOV)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
  add_definitions(-DGCOV_ENABLED)
  message(STATUS "GCOV enabled")
endif()

set(CMAKE_CXX_FLAGS_RELEASE " -DNDEBUG -s -O2")
set(CMAKE_CXX_FLAGS_DEBUG " -DDEBUG -g3 -ggdb3")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_definitions(-DOS_LINUX)
elseif(CMAKE_SYSTEM_NAME STREQUAL "QNX")
  add_definitions(-DOS_QNX)
  set(BUILD_BT_SUPPORT OFF)
  set(BUILD_BACKTRACE_SUPPORT OFF)
  set(EXTENDED_MEDIA_MODE OFF)
endif()

if(BUILD_BACKTRACE_SUPPORT)
  add_definitions(-DBACKTRACE_SUPPORT)
  message(STATUS "Backtrace support enabled")
endif()

if(ENABLE_LOG)
  add_definitions(-DENABLE_LOG)
  set(install-3rd_party_logger "install-3rd_party_logger")
  message(STATUS "Logger enabled")
endif()

set(install-bson_c_lib "install-bson_c_lib")

if(TELEMETRY_MONITOR)
  add_definitions(-DTELEMETRY_MONITOR)
  message(STATUS "Telemetry monitor enabled")
endif()

add_definitions(-DOS_POSIX)

if(EXTENDED_MEDIA_MODE)
  add_definitions(-DEXTENDED_MEDIA_MODE)
  message(STATUS "Extended media mode enabled")
  # required to find 'glibconfig.h'
  find_package(PkgConfig)
  pkg_check_modules(GLIB2 REQUIRED glib-2.0)
  add_definitions(${GLIB2_CFLAGS})
endif()

# Building application

# --- Type HMI
if(HMI STREQUAL "web")
  set(WEB_HMI ON)
  add_definitions(-DWEB_HMI)
else()
  set(HMI "no")
  add_definitions(-DNO_HMI)
endif()

if(HMI STREQUAL "web" AND NOT DEFINED HMIADAPTER)
  set(HMIADAPTER "messagebroker")
endif()

if(HMIADAPTER STREQUAL "messagebroker")
  set(HMI_JSON_API ON)
  add_definitions(-DMESSAGEBROKER_HMIADAPTER)
  add_definitions(-DHMI_JSON_API)
endif()

if(ENABLE_SECURITY)
  add_definitions(-DENABLE_SECURITY)
  set(SecurityManagerLibrary SecurityManager)
  set(SecurityManagerIncludeDir ${COMPONENTS_DIR}/security_manager/include)
  message(STATUS "Security enabled")
endif()

if(ENABLE_HMI_PTU_DECRYPTION)
  add_definitions(-DUSE_HMI_PTU_DECRYPTION)
  message(STATUS "HMI PTU decription enabled")
endif()

set(RTLIB rt)
if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
  set(RTLIB)
endif()

set(RPATH_DIRECTORIES
    ${CMAKE_INSTALL_PREFIX}/bin/plugins
    /usr/local/lib
    /usr/local
    ${CMAKE_INSTALL_PREFIX}/bin)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${RPATH_DIRECTORIES}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Building tests
if(BUILD_TESTS)
  enable_testing()
  add_definitions(-DBUILD_TESTS)
  include_directories(${COMPONENTS_DIR}/include/test)
  message(STATUS "Tests enabled")
endif()

# Building documentation

# At first creating directory for generated documentation. Unfortunately doxygen
# cannot generate it byself
find_package(Doxygen)
if(DOXYGEN_FOUND)
  option(DOXYGEN_ENABLE_DIAGRAMS "Enable graphical diagram generation" ON)
  message(STATUS "Doxygen found. Documentation will be generated")

  if(DOXYGEN_ENABLE_DIAGRAMS)
    set(DOXYGEN_ENABLE_DIAGRAMS_PARAM "YES")
  else()
    set(DOXYGEN_ENABLE_DIAGRAMS_PARAM "NO")
  endif()
  configure_file("${PROJECT_SOURCE_DIR}/Doxyfile"
                 "${PROJECT_BINARY_DIR}/Doxyfile")
  file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/doc/doxygen")
  add_custom_target(doxygen
                    COMMAND ${DOXYGEN_EXECUTABLE}
                            "${PROJECT_BINARY_DIR}/Doxyfile")
else()
  message(STATUS "Doxygen not found. Documentation will not be generated")
  message(
    STATUS
      "To enable documentation generation please install doxygen and graphviz packages"
    )
  message(STATUS "sudo apt-get install doxygen graphviz")
  message(
    STATUS "To enable processing of MscGen comments please install mscgen")
  message(STATUS "sudo apt-get install mscgen")
endif()

add_subdirectory(src)
