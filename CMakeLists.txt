# Copyright (c) 2016, Ford Motor Company
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

cmake_minimum_required(VERSION 2.8.11)

set(PROJECT smartDeviceLinkCore)
project(${PROJECT})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/tools/cmake/modules/")

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/platform.cmake)

# Check build settings
get_os(OS)
message(STATUS "Operation system: " ${OS})

get_arch(ARCH)
message(STATUS "Architecture: " ${ARCH})

get_sdk(SDK)
if(SDK)
  message(STATUS "SDK: " ${SDK})
endif()

# Build options
option(QT_HMI "Use Qt HMI" OFF)
option(EXTENDED_MEDIA_MODE "Turn on and off extended Media Manager features relates to GStreamer" OFF)
option(BUILD_SHARED_LIBS "Build all libraries as shared (if ON) or static (if OFF)" OFF)
option(BUILD_BT_SUPPORT "Bluetooth support" ON)
option(BUILD_USB_SUPPORT "libusb support" ON)
option(BUILD_BACKTRACE_SUPPORT "backtrace support" OFF)
option(BUILD_TESTS "Possibility to build and run tests" ON)
option(TELEMETRY_MONITOR "Enable profiling time test util" OFF)
option(ENABLE_LOG "Logging feature" ON)
option(ENABLE_GCOV "gcov code coverage feature" OFF)
option(ENABLE_SANITIZE "Sanitize tool" OFF)
option(ENABLE_SECURITY "Security Ford protocol protection" ON)
option(ENABLE_HMI_PTU_DECRYPTION "Policy table update parsed by hmi" ON)
option(ENABLE_EXTENDED_POLICY "Turns extended flow which requires embedded system interaction" ON)
option(ENABLE_THREAD_COUNT "Turn on correct threads shutdown checks" OFF)
option(FAST_BUILD "Turn on fast build" ON)
option(USE_CCACHE "Turn on ccache usage" ON)
option(USE_DISTCC "Turn on distributed build_usage" ON)

# Platform - specific build flags
if(${OS} STREQUAL "posix")
  add_definitions(-DOS_POSIX)
  add_definitions(-DOS_LINUX)

  set(CMAKE_CXX_FLAGS "-fPIC -std=gnu++0x -Wall -Werror -Wno-deprecated-declarations -Wuninitialized -Wvla")
  set(CMAKE_CXX_FLAGS_RELEASE " -DNDEBUG -s -O2")
  set(CMAKE_CXX_FLAGS_DEBUG   " -DDEBUG -g3 -ggdb3")
  set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

  if(ENABLE_LOG)
    set(LOG4CXX_LOGGER ON)
    add_definitions(-DLOG4CXX_LOGGER)
  endif()

  set(INSTALL_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
elseif(${OS} STREQUAL "win")
  add_definitions(-DOS_WINDOWS)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /GF /GR /EHsc /W3 /D_CONSOLE /bigobj")
  # /WD4996 - disable warning C4996.
  # 'inet_ntoa': Use inet_ntop() or InetNtop() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
  # to disable deprecated API warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
  # /WD4503 - disable warning C4503.
  # xmemory0(592): warning C4503: 'std::_Tree<std::_Tmap_traits<_Kty,_Ty,_Pr,_Alloc,false>>::_Insert_nohint' :
  # decorated name length exceeded, name was truncated
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4503")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD -DNDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd -DDEBUG")
  # /NOLOGO option prevents display of the copyright message and version number
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /nologo")
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)

  if(SDK)
    if(${SDK} STREQUAL "qt")
      add_definitions(-DQT_PORT)
	  
      set(CMAKE_AUTOMOC ON)
      find_package(Qt5 REQUIRED Core Network)
      include_directories(
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS}
      )

      #Add MSVS 2010 for Qt specific flags
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32")
    else()
      # Add MSVS x64 specific flags
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN64 /Zc:wchar_t /favor:blend")
    endif()
  else()
    add_definitions(-DWIN_NATIVE)
  endif()

  set(INSTALL_DIRECTORY ${CMAKE_BINARY_DIR}/src/appMain)
endif()

if(QT_HMI)
  set(HMI_DBUS_API ON)
  add_definitions(-DDBUS_HMIADAPTER)
  add_definitions(-DHMI_DBUS_API)
  message(STATUS "Qt HMI support enabled")
else()
  set(HMI_JSON_API ON)
  add_definitions(-DMESSAGEBROKER_HMIADAPTER)
  add_definitions(-DHMI_JSON_API)
  message(STATUS "Web HMI support enabled")
endif()

if(BUILD_USB_SUPPORT)
  add_definitions(-DUSB_SUPPORT)
  message(STATUS "USB support enabled")
endif()

if(BUILD_BT_SUPPORT)
  add_definitions(-DBLUETOOTH_SUPPORT)
  message(STATUS "Bluetooth support enabled")
endif()

if(BUILD_BACKTRACE_SUPPORT)
  add_definitions(-DBACKTRACE_SUPPORT)
  message(STATUS "Backtrace support enabled")
endif()

if(ENABLE_LOG)
  add_definitions(-DENABLE_LOG)
  message(STATUS "Logger enabled")
endif()

if(TELEMETRY_MONITOR)
  add_definitions(-DTELEMETRY_MONITOR)
  message(STATUS "Telemetry monitor enabled")
endif()

if(ENABLE_EXTENDED_POLICY)
  add_definitions(-DEXTENDED_POLICY)
  message(STATUS "Extended policy enabled")
endif()

if(ENABLE_THREAD_COUNT)
  add_definitions(-DTHREAD_COUNT)
  message(STATUS "Thread counting enabled")
endif()

if (USE_CCACHE)
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

if (USE_DISTCC)
# Configure distributed compilation if available
  find_program(DISTCC_FOUND distcc)
  if(DISTCC_FOUND)
    get_property(RULE_LAUNCH_COMPILE GLOBAL PROPERTY RULE_LAUNCH_COMPILE)
    set(RULE_LAUNCH_COMPILE "${RULE_LAUNCH_COMPILE} distcc")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${RULE_LAUNCH_COMPILE})
    message(INFO " Used distributed build.")
  else(CCACHE_FOUND)
    message(WARNING " distcc was not found. Distributed compilation is impossible.")
  endif(DISTCC_FOUND)
endif(USE_DISTCC)

if(EXTENDED_MEDIA_MODE)
  add_definitions(-DEXTENDED_MEDIA_MODE)
  message(STATUS "Extended media mode enabled")
endif()

if(ENABLE_SANITIZE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  message(STATUS "Sanitizing enabled")
endif()

if(ENABLE_GCOV)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
  add_definitions(-DGCOV_ENABLED)
  message(STATUS "GCOV enabled")
endif()

if(ENABLE_SECURITY)
  add_definitions(-DENABLE_SECURITY)
  message(STATUS "Security enabled")
endif()

if(ENABLE_HMI_PTU_DECRYPTION)
  add_definitions(-DUSE_HMI_PTU_DECRYPTION)
  message(STATUS "HMI PTU decription enabled")
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/components/include
)

include_directories(
  $ENV{THIRD_PARTY_INSTALL_PREFIX}/include
)

if(BUILD_TESTS)
  enable_testing()
  add_definitions(-DBUILD_TESTS)
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/components/include/test
  )
  message(STATUS "Tests enabled")
endif()

# --- 3rd party libs
add_subdirectory(./src/3rd_party EXCLUDE_FROM_ALL)

# --- 3rd party libs (static)
add_subdirectory(./src/3rd_party-static)

# --- Tools
add_subdirectory(./tools)

# --- Components
add_subdirectory(./src/components)

# --- Main application
add_subdirectory(./src/appMain)

# --- Plugins
add_subdirectory(./src/plugins)

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
    configure_file("${CMAKE_SOURCE_DIR}/Doxyfile" "${CMAKE_BINARY_DIR}/Doxyfile")
    file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/doc/doxygen")
    add_custom_target(doxygen COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile")
  else()
    message(STATUS "Doxygen not found. Documentation will not be generated")
endif()
