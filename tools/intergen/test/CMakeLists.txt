# Copyright (c) 2015, Ford Motor Company
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
if(BUILD_TESTS)
include(${CMAKE_SOURCE_DIR}/tools/intergen/GenerateInterfaceLibrary.cmake)

if (${HMI_DBUS_API})
  GenerateInterfaceLibrary("test_interface.xml" test_rpc_interface DBUS_SUPPORT)
else()
  GenerateInterfaceLibrary("test_interface.xml" test_rpc_interface)
endif()

set (TEST_HMI_INTERFACES
  common
  buttons
  navigation
  basic_communication
  tts
  ui
)

if (${HMI_DBUS_API})
  GenerateInterfaceLibrary("test_hmi_interface.xml" "${TEST_HMI_INTERFACES}" AUTO_FUNC_IDS DBUS_SUPPORT)
else()
  GenerateInterfaceLibrary("test_hmi_interface.xml" "${TEST_HMI_INTERFACES}" AUTO_FUNC_IDS)
endif()

include_directories (
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${GMOCK_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    test_rpc_interface
)

set (SOURCES
  generated_interface_json_tests.cc
)

if (${HMI_DBUS_API})
  # Build dbus tests
  include_directories(
      ${CMAKE_SOURCE_DIR}/src/components/dbus/include
      ${DBUS_INCLUDE_DIRS}
  )
  set (LIBRARIES ${LIBRARIES} DBus)
  set (SOURCES ${SOURCES} generated_interface_dbus_tests.cc)
endif ()


create_test(test_generated_interface "${SOURCES}" "${LIBRARIES}")
endif()  