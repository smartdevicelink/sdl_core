# Copyright (c) 2017, Ford Motor Company
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

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

include_directories(
  include
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${COMPONENTS_DIR}/smart_objects/include
  ${COMPONENTS_DIR}/transport_manager/include
  ${COMPONENTS_DIR}/utils/include
  ${COMPONENTS_DIR}/connection_handler/include
  ${JSONCPP_INCLUDE_DIRECTORY}
)

set(EXCLUDE_PATHS
  raw_message_matcher.cc
)

collect_sources(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}" "${EXCLUDE_PATHS}")

set(LIBRARIES
  gmock
  ConfigProfile
  TransportManager
  Utils
  ConfigProfile
  ProtocolLibrary
  Resumption
  jsoncpp
)

if (BUILD_USB_SUPPORT)
  list(APPEND LIBRARIES libusb-1.0.so)
endif()

if (BUILD_BT_SUPPORT)
 if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND LIBRARIES bluetooth)
 endif()
endif()

create_test("transport_manager_test" "${SOURCES}" "${LIBRARIES}")

file(COPY smartDeviceLink_test.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
