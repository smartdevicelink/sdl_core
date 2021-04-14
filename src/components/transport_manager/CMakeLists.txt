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

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/platform.cmake)
include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

include_directories (
  include
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/protocol_handler/include
  ${COMPONENTS_DIR}/connection_handler/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/resumption/include
  ${POLICY_GLOBAL_INCLUDE_PATH}/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(EXCLUDE_PATHS)

set(LIBRARIES
  ProtocolLibrary
  Utils
)

if(BUILD_BT_SUPPORT)
  list(APPEND LIBRARIES
    bluetooth
  )
else()
  list(APPEND EXCLUDE_PATHS
    ${COMPONENTS_DIR}/transport_manager/include/transport_manager/bluetooth
    ${COMPONENTS_DIR}/transport_manager/src/bluetooth
  )
endif()

if(BUILD_USB_SUPPORT)
  if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(EXCLUDE_PATHS
      ${COMPONENTS_DIR}/transport_manager/include/transport_manager/usb/qnx
      ${COMPONENTS_DIR}/transport_manager/src/usb/qnx
    )

  elseif(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    set(EXCLUDE_PATHS
      ${COMPONENTS_DIR}/transport_manager/include/transport_manager/usb/libusb
      ${COMPONENTS_DIR}/transport_manager/src/usb/libusb
    )
    list(APPEND LIBRARIES
      usbdi
    )
  endif()
else()
  list(APPEND EXCLUDE_PATHS
    ${COMPONENTS_DIR}/transport_manager/include/transport_manager/usb
    ${COMPONENTS_DIR}/transport_manager/src/usb
  )
endif()

if(NOT BUILD_TESTS)
  list (APPEND EXCLUDE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/include/iap2_emulation/iap2_transport_adapter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/iap2_emulation/iap2_transport_adapter.cc
  )
endif()

collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")

add_library("TransportManager" ${SOURCES})
target_link_libraries("TransportManager" ${LIBRARIES})

if(ENABLE_LOG)
  target_link_libraries("TransportManager" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
