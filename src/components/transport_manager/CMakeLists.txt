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

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include/transport_manager/tcp/platform_specific/qnx")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include/transport_manager/tcp/platform_specific/linux")
endif ()

set(LIBRARIES
  ProtocolLibrary
  Utils
)

set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/transport_manager_default.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/transport_manager_impl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/network_interface_listener_impl.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/tcp_client_listener.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/tcp_connection_factory.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/tcp_device.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/tcp_server_originated_socket_connection.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/tcp_socket_connection.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/tcp_transport_adapter.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/tcp_transport_adapter.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/src/transport_adapter/threaded_socket_connection.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/transport_adapter/transport_adapter_impl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/transport_adapter/transport_adapter_listener_impl.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/src/iap2_emulation/iap2_transport_adapter.cc

)

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  list(APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/platform_specific/qnx/platform_specific_network_interface_listener.cc
  )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp/platform_specific/linux/platform_specific_network_interface_listener.cc
  )
endif ()

if(BUILD_BT_SUPPORT)
  list(APPEND LIBRARIES
    bluetooth
  )
  list(APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bluetooth/bluetooth_connection_factory.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bluetooth/bluetooth_device.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bluetooth/bluetooth_device_scanner.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bluetooth/bluetooth_socket_connection.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bluetooth/bluetooth_transport_adapter.cc
  )
endif()

if(BUILD_USB_SUPPORT)
  list(APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/usb_aoa_adapter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/usb_connection_factory.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/usb_device_scanner.cc
  )
  if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND SOURCES
      ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/libusb/platform_usb_device.cc
      ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/libusb/usb_connection.cc
      ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/libusb/usb_handler.cc
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    list(APPEND LIBRARIES
      usbdi
    )
    list(APPEND SOURCES
      ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/qnx/platform_usb_device.cc
      ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/qnx/usb_connection.cc
      ${CMAKE_CURRENT_SOURCE_DIR}/src/usb/qnx/usb_handler.cc
    )
  endif()
endif()


add_library("TransportManager" ${SOURCES})
target_link_libraries("TransportManager" ${LIBRARIES})

if(ENABLE_LOG)
  target_link_libraries("TransportManager" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
