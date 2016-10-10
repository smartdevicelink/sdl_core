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

cmake_minimum_required(VERSION 2.8.0)

set (PROJECT smartDeviceLinkCore)

#Jenkins integration section
#dont modify this section if you dont know details about integration with Jenkins!!!
set (HMI "web" CACHE STRING "HMI type")
option(HMI2 "Use Qt HMI" OFF)
option(EXTENDED_MEDIA_MODE "Turn on and off extended Madia Manager features relates to PulseAudio A2DP and GStreamer" OFF)
option(BUILD_SHARED_LIBS "Build all libraries as shared (if ON) or static (if OFF)" OFF)
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
option(ENABLE_EXTENDED_POLICY "Turns extended flow which requires embedded system interaction" ON)
option(USE_COTIRE "Use Cotire to speed up build (currently only for commands tests)" ON)
option(USE_GOLD_LD "Use gold linker intead of GNU linker" ON)
option(USE_CCACHE "Turn on ccache usage" ON)
option(USE_DISTCC "Turn on distributed build_usage" ON)

set(OS_TYPE_OPTION 	"$ENV{OS_TYPE}")
set(DEBUG_OPTION 	"$ENV{DEBUG}")
set(HMI_TYPE_OPTION 	"$ENV{HMI_TYPE}")
set(TARGET_OPTION 	"$ENV{TARGET}")
set(MEDIA_MODE_OPTION 	"$ENV{MEDIA_MODE}")
set(HMI_ADAPTER_OPTION 	"$ENV{HMI_ADAPTER}")
set(ENABLE_TESTS_OPTION "$ENV{ENABLE_TESTS}")
set(ENABLE_LOG_OPTION 	"$ENV{ENABLE_LOG}")
set(ARCH_TYPE_OPTION	"$ENV{ARCH_TYPE}")
set(POLICY_OPTION 	"$ENV{POLICY_TYPE}")
set(SECURITY_OPTION 	"$ENV{SECURITY_MODE}")
set(COMPONENTS_DIR ${CMAKE_SOURCE_DIR}/src/components)
set(SNAPSHOT_TAG 	"$ENV{SNAPSHOT_TAG}")

set(CMAKE_CXX_COMPILER   $ENV{CMAKE_CXX_COMPILER})
set(CMAKE_C_COMPILER     $ENV{CMAKE_C_COMPILER})

if (ARCH_TYPE_OPTION)
  if (NOT (${ARCH_TYPE_OPTION} STREQUAL "x86") AND NOT (${ARCH_TYPE_OPTION} STREQUAL "armv7"))
    message(AUTHOR_WARNING "HW architecture is not defined, using x86. Allowed values are x86/armv7 (case sensitive)")
    set(ARCH_TYPE_OPTION	"x86")
  endif ()
else ()
    set(ARCH_TYPE_OPTION	"x86")
endif()

if (HMI_TYPE_OPTION)
  if (${HMI_TYPE_OPTION} STREQUAL "HTML5")
    message(STATUS "Jenkins integration: select HTML5 HMI")
    set (HMI "web")
  elseif (${HMI_TYPE_OPTION} STREQUAL "NONE")
    message(STATUS "Jenkins integration: select HMI none")
    set (HMI "no")
  else ()
    message(STATUS "Jenkins integration: select QML HMI none")
    set (HMI "qt")
  endif()
endif()

if (MEDIA_MODE_OPTION)
  if (${MEDIA_MODE_OPTION} STREQUAL "EXTENDED_MEDIA")
    message(STATUS "Jenkins integration: select extended media mode")
    set (EXTENDED_MEDIA_MODE ON)
  endif()
endif()

if (DEBUG_OPTION)
  if (${DEBUG_OPTION} STREQUAL "DBG_OFF")
    message(STATUS "Jenkins integration: build release version")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
  endif()
endif()

if (HMI_ADAPTER_OPTION)
  if (${HMI_ADAPTER_OPTION} STREQUAL "MESSAGEBROKER")
    message(STATUS "Jenkins integration: selected HMI adapter MESSAGEBROKER")
    set (HMIADAPTER "messagebroker")
  elseif (${HMI_ADAPTER_OPTION} STREQUAL "DBUS")
    message(STATUS "Jenkins integration: selected HMI adapter DBUS")
    set (HMIADAPTER "dbus")
  elseif (${HMI_ADAPTER_OPTION} STREQUAL "MQUEUE")
    message(STATUS "Jenkins integration: selected HMI adapter MQUEUE")
    set (HMIADAPTER "mqueue")
  endif()
endif()

if (ENABLE_LOG_OPTION)
  if (${ENABLE_LOG_OPTION} STREQUAL "LOG_OFF")
    message(STATUS "Jenkins integration: Log is turned off")
    set (ENABLE_LOG OFF)
  endif()
endif()

if (ENABLE_TESTS_OPTION)
  if (${ENABLE_TESTS_OPTION} STREQUAL "TESTS_OFF")
    message(STATUS "Jenkins integration: Unit tests is turned off")
    set (BUILD_TESTS OFF)
  elseif(${ENABLE_TESTS_OPTION} STREQUAL "TESTS_ON")
    message(STATUS "Jenkins integration: Unit tests is turned on")
    set (BUILD_TESTS ON)
  endif()
endif()

if (SECURITY_OPTION)
  if (${SECURITY_OPTION} STREQUAL "SEC_OFF")
    message(STATUS "Jenkins integration: Security is turned OFF")
    set (ENABLE_SECURITY OFF)
  endif()
endif()

set(objcopy "objcopy")
if (OS_TYPE_OPTION)
  if (${OS_TYPE_OPTION} STREQUAL "QNX")
    message(STATUS "Jenkins integration: set build process for QNX")
    #do not use include after project() command.
    #Such usecase results in infinite cycle of reinitialization of compiler and other variables
    INCLUDE("./qnx_6.5.0_linux_x86.cmake")
    set(objcopy "nto${ARCH_TYPE_OPTION}-objcopy")
    #tests are not supported yet for QNX build
    set (BUILD_TESTS OFF)
  endif()
endif()

#Jenkins integration section end

add_custom_target(pasa-tarball
  COMMAND ${CMAKE_SOURCE_DIR}/tools/Utils/export-customer-specific.sh ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} pasa
  COMMAND tar -cz -C /tmp/PASA -f ${CMAKE_BINARY_DIR}/pasa.tar.gz .
  DEPENDS HMI_API MOBILE_API v4_protocol_v1_2_no_extra
)

add_custom_target(ford-tarball
  COMMAND ${CMAKE_SOURCE_DIR}/tools/Utils/export-customer-specific.sh ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ford
  COMMAND tar -cz -C /tmp/FORD -f ${CMAKE_BINARY_DIR}/ford.tar.gz .
  DEPENDS HMI_API MOBILE_API v4_protocol_v1_2_no_extra
)

add_custom_target(genivi-tarball
  COMMAND ${CMAKE_SOURCE_DIR}/tools/Utils/export-customer-specific.sh ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} genivi
  COMMAND tar -cz -C /tmp/GENIVI -f ${CMAKE_BINARY_DIR}/genivi.tar.gz .
)


project (${PROJECT})

#ADD_DEPENDENCIES(${PROJECT} Policy)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Please do not change compiler/linker flags if You do not know how particular
# flag is handled by CMake
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

set(CMAKE_CXX_FLAGS "-fPIC -std=gnu++0x -Wall -Werror -Wno-deprecated-declarations -Wuninitialized -Wvla")

if (USE_GOLD_LD)
  execute_process(COMMAND ld -v OUTPUT_VARIABLE result)
  string(FIND ${result} Binutils GOLD_LD_FOUND)
  if (GOLD_LD_FOUND EQUAL -1)
    message(SEND_ERROR "Gold linker not found!")
  else()
    message(INFO " Use gold linker")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
  endif()
endif(USE_GOLD_LD)

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

if(ENABLE_SANITIZE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()
if(ENABLE_GCOV)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
  add_definitions(-DGCOV_ENABLED)
endif()
set(CMAKE_CXX_FLAGS_RELEASE " -DNDEBUG -s -O2")
set(CMAKE_CXX_FLAGS_DEBUG   " -DDEBUG -g3 -ggdb3")

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_definitions(-DOS_LINUX)
elseif(CMAKE_SYSTEM_NAME STREQUAL "QNX")
  add_definitions(-DOS_QNX)
  SET(BUILD_BT_SUPPORT OFF)
  SET(BUILD_BACKTRACE_SUPPORT OFF)
  SET(EXTENDED_MEDIA_MODE OFF)
endif()


if (BUILD_USB_SUPPORT)
  add_definitions(-DUSB_SUPPORT)
  find_package(UDev REQUIRED)
  message(STATUS "USB support is enabled")
endif()


if (BUILD_BT_SUPPORT)
  add_definitions(-DBLUETOOTH_SUPPORT)
  message(STATUS "Bluetooth support is enabled")
endif()

if (BUILD_BACKTRACE_SUPPORT)
  add_definitions(-DBACKTRACE_SUPPORT)
endif()

if(ENABLE_LOG)
  add_definitions(-DENABLE_LOG)
  set(install-3rd_party_logger "install-3rd_party_logger")
endif()

if (TELEMETRY_MONITOR)
    add_definitions(-DTELEMETRY_MONITOR)
endif()

if (ENABLE_EXTENDED_POLICY)
    add_definitions(-DEXTENDED_POLICY)
endif()

# TODO(AK): check current OS here
add_definitions(-DOS_POSIX)

if (EXTENDED_MEDIA_MODE)
add_definitions(-DEXTENDED_MEDIA_MODE)
# required to find 'glibconfig.h'
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})
endif()


# --- Interface generator

find_package(PythonInterp)
if(NOT PYTHONINTERP_FOUND)
    message(STATUS "Python interpreter is not found")
    message(STATUS "To install it type in the command line:")
    message(STATUS "sudo apt-get install python")
    message(FATAL_ERROR "Exiting!")
endif(NOT PYTHONINTERP_FOUND)

if (HMI STREQUAL "qt")
    cmake_minimum_required(VERSION 2.8.11)
    if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
        set(qt_version "4.8.5")
    else ()
        set(qt_version "5.1.0")
    endif ()

    execute_process(
        COMMAND ${CMAKE_SOURCE_DIR}/FindQt.sh -v ${qt_version}
            OUTPUT_VARIABLE qt_bin_dir
    )
    message(STATUS "Binary directory Qt ${qt_version} is ${qt_bin_dir}")
    set(ENV{PATH} ${qt_bin_dir}:$ENV{PATH})

    if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
        find_package(Qt4 ${qt_version} REQUIRED QtCore QtGui QtDBus QtDeclarative)
    else ()
        find_package(Qt5Core REQUIRED)
        find_package(Qt5DBus REQUIRED)
        find_package(Qt5Qml REQUIRED)
        find_package(Qt5Quick REQUIRED)
        set(qmlplugindump_binary ${qt_bin_dir}/qmlplugindump)
    endif ()
endif()

set(INTEFRACE_GENERATOR "${PROJECT_SOURCE_DIR}/tools/InterfaceGenerator/Generator.py")
set(INTEFRACE_GENERATOR_CMD ${PYTHON_EXECUTABLE} -B ${INTEFRACE_GENERATOR})
file(GLOB_RECURSE INTERFACE_GENERATOR_DEPENDENCIES "${PROJECT_SOURCE_DIR}/tools/InterfaceGenerator/*.*")

macro (GenerateInterface arg_xml_name arg_namespace parser_type)
    string(REGEX MATCH "^[a-zA-Z_0-9]*[^.]" file_name ${arg_xml_name})     # TODO: make expression more robust

    set(hpp_file
      "${CMAKE_CURRENT_BINARY_DIR}/${file_name}.h"
      "${CMAKE_CURRENT_BINARY_DIR}/${file_name}_schema.h"
    )

    set(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/${file_name}_schema.cc")
    set(full_xml_name "${CMAKE_CURRENT_SOURCE_DIR}/${arg_xml_name}")

    add_custom_command( OUTPUT ${hpp_file} ${cpp_file}
                        COMMAND ${INTEFRACE_GENERATOR_CMD} ${full_xml_name} ${arg_namespace} ${CMAKE_CURRENT_BINARY_DIR} "--parser-type" "${parser_type}"
                        DEPENDS ${INTERFACE_GENERATOR_DEPENDENCIES} ${full_xml_name}
                        COMMENT "Generating files:\n   ${hpp_file}\n   ${cpp_file}\nfrom:\n   ${arg_xml_name} ..."
                        VERBATIM
                       )

    include_directories (
        ${COMPONENTS_DIR}/smart_objects/include
        ${COMPONENTS_DIR}/formatters/include/
        ${CMAKE_BINARY_DIR}
    )

    add_library("${file_name}" ${cpp_file})
endmacro(GenerateInterface)

# --- Useful macro
macro(create_test NAME SOURCES LIBS)
  add_executable("${NAME}" ${CMAKE_SOURCE_DIR}/src/components/test_main.cc ${SOURCES})
  target_link_libraries("${NAME}" ${LIBS})
  target_link_libraries("${NAME}" Utils)
  if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    add_test(${NAME} ${CMAKE_SOURCE_DIR}/qnx/remote_run_test.sh ${NAME})
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_test(NAME ${NAME}
    COMMAND ${NAME} --gtest_output=xml:${CMAKE_BINARY_DIR}/test_results/)
  else()
    add_test(${NAME} ${NAME})
  endif()
endmacro(create_test)

# --replace in list macro
macro(LIST_REPLACE LIST INDEX NEWVALUE)
    list(INSERT ${LIST} ${INDEX} ${NEWVALUE})
    MATH(EXPR __INDEX "${INDEX} + 1")
    list (REMOVE_AT ${LIST} ${__INDEX})
endmacro(LIST_REPLACE)


# Building application

# --- Type HMI
if (HMI STREQUAL "qt")
    set(QT_HMI ON)
    add_definitions(-DQT_HMI)
elseif (HMI STREQUAL "web")
    set(WEB_HMI ON)
    add_definitions(-DWEB_HMI)
else ()
    set(HMI "no")
    add_definitions(-DNO_HMI)
endif ()

if (HMI STREQUAL "qt" AND NOT DEFINED HMIADAPTER)
    set(HMIADAPTER "dbus")
endif()
if (HMI STREQUAL "web" AND NOT DEFINED HMIADAPTER)
    set(HMIADAPTER "messagebroker")
endif()
if (HMI STREQUAL "no" AND NOT DEFINED HMIADAPTER)
    set(HMIADAPTER "mqueue")
endif()

if (HMIADAPTER STREQUAL "dbus")
    set(HMI_DBUS_API ON)
    add_definitions(-DDBUS_HMIADAPTER)
    add_definitions(-DHMI_DBUS_API)
    set(install-3rd_party_dbus "install-3rd_party_dbus")
endif()
if (HMIADAPTER STREQUAL "messagebroker")
    set(HMI_JSON_API ON)
    add_definitions(-DMESSAGEBROKER_HMIADAPTER)
    add_definitions(-DHMI_JSON_API)
endif()
if (HMIADAPTER STREQUAL "mqueue")
    set(HMI_JSON_API ON)
    add_definitions(-DMQUEUE_HMIADAPTER)
    add_definitions(-DHMI_JSON_API)
endif()

# --- Directory with SDL interfaces, global types and ProtocolLib component
include_directories(
  ${COMPONENTS_DIR}/include
  ${COMPONENTS_DIR}/protocol/include
if (BUILD_TESTS)
  ${COMPONENTS_DIR}/include/test
endif ()
)

# --- 3rd party libs
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/src/3rd_party/set_3rd_party_paths.cmake)

set(3RD_PARTY_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/3rd_party)
set(3RD_PARTY_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/3rd_party)

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

    set (install-3rd_party_logger_var "install-3rd_party_logger")
  endif()
endif()

if (HMIADAPTER STREQUAL "dbus")
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
    endif()

    set (install-3rd_party_dbus_var "install-3rd_party_dbus")
  endif()
endif()

add_subdirectory(${3RD_PARTY_SOURCE_DIRECTORY} ${3RD_PARTY_BINARY_DIRECTORY} EXCLUDE_FROM_ALL)
add_custom_target(install-3rd_party
  DEPENDS ${install-3rd_party_logger_var}
  DEPENDS ${install-3rd_party_dbus_var}
  WORKING_DIRECTORY ${3RD_PARTY_BINARY_DIRECTORY}
)

find_package(OpenSSL REQUIRED)
if(ENABLE_LOG)
  include_directories ( ${LOG4CXX_INCLUDE_DIRECTORY} )
endif()

if(ENABLE_SECURITY)
  add_definitions(-DENABLE_SECURITY)
  set(SecurityManagerLibrary SecurityManager)
  set(SecurityManagerIncludeDir ${COMPONENTS_DIR}/security_manager/include)
  #set(SecurityManagerTestIncludeDir ${CMAKE_SOURCE_DIR}/test/components/security_manager/include)
endif()

if(ENABLE_HMI_PTU_DECRYPTION)
  MESSAGE("USE DHMI_PTU_PARSER")
  add_definitions(-DUSE_HMI_PTU_DECRYPTION)
endif()

set(RTLIB rt)
if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
set(RTLIB )
endif()

# Building tests
if(BUILD_TESTS)
  enable_testing()
  add_definitions(-DBUILD_TESTS)
  # Framework GoogleTest is also integrated together gmock
  # and must not be added separately
  add_subdirectory(./src/3rd_party-static/gmock-1.7.0)
endif()

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


# Building tests
if(BUILD_TESTS)
    # Directory test is deprecated. Use src/components/<name>/test
    include(Dart)
    #add_subdirectory(./test)
endif()

# Building documentation

# At first creating directory for generated documentation. Unfortunately doxygen
# cannot generate it byself
FIND_PACKAGE(Doxygen)
  IF(DOXYGEN_FOUND)
    option(DOXYGEN_ENABLE_DIAGRAMS "Enable graphical diagram generation" ON)

    if(DOXYGEN_ENABLE_DIAGRAMS)
      set(DOXYGEN_ENABLE_DIAGRAMS_PARAM "YES")
    else(DOXYGEN_ENABLE_DIAGRAMS)
      set(DOXYGEN_ENABLE_DIAGRAMS_PARAM "NO")
    endif()
    configure_file("${PROJECT_SOURCE_DIR}/Doxyfile" "${PROJECT_BINARY_DIR}/Doxyfile")
    file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/doc/doxygen")
    ADD_CUSTOM_TARGET(doxygen COMMAND ${DOXYGEN_EXECUTABLE} "${PROJECT_BINARY_DIR}/Doxyfile")
  ELSE(DOXYGEN_FOUND)
    MESSAGE(STATUS "Doxygen not found. Documentation will not be generated")
    MESSAGE(STATUS "To enable documentation generation please install doxygen and graphviz packages")
    MESSAGE(STATUS "sudo apt-get install doxygen graphviz")
    MESSAGE(STATUS "To enable processing of MscGen comments please install mscgen")
    MESSAGE(STATUS "sudo apt-get install mscgen")
ENDIF(DOXYGEN_FOUND)

