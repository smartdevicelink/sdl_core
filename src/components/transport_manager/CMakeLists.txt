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

find_package(LibUSBx REQUIRED)

set(target TransportManager)
set(TM_SRC_DIR ${COMPONENTS_DIR}/transport_manager/src)
set(TM_INC_DIR ${COMPONENTS_DIR}/transport_manager/include/transport_manager)

include_directories (
  include
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/protocol_handler/include
  ${COMPONENTS_DIR}/connection_handler/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/resumption/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${LIBUSBX_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set (inc
    ${TM_INC_DIR}/telemetry_observer.h
    ${TM_INC_DIR}/transport_manager_default.h
    ${TM_INC_DIR}/transport_manager_impl.h
   )
  set (inc_tm_adpt
    ${TM_INC_DIR}/transport_adapter/client_connection_listener.h
    ${TM_INC_DIR}/transport_adapter/connection.h
    ${TM_INC_DIR}/transport_adapter/device_scanner.h
    ${TM_INC_DIR}/transport_adapter/server_connection_factory.h
    ${TM_INC_DIR}/transport_adapter/threaded_socket_connection.h
    ${TM_INC_DIR}/transport_adapter/transport_adapter_controller.h
    ${TM_INC_DIR}/transport_adapter/transport_adapter_impl.h
    ${TM_INC_DIR}/transport_adapter/transport_adapter_listener.h
    ${TM_INC_DIR}/transport_adapter/transport_adapter_listener_impl.h
   )
  set(inc_tcp
    ${TM_INC_DIR}/tcp/tcp_client_listener.h
    ${TM_INC_DIR}/tcp/tcp_connection_factory.h
    ${TM_INC_DIR}/tcp/tcp_device.h
    ${TM_INC_DIR}/tcp/tcp_socket_connection.h
    ${TM_INC_DIR}/tcp/tcp_transport_adapter.h
  )
  set (src
    ${TM_SRC_DIR}/transport_manager_impl.cc
    ${TM_SRC_DIR}/transport_manager_default.cc
    ${TM_SRC_DIR}/tcp/tcp_transport_adapter.cc
   )
  set (src_tm_adpt
    ${TM_SRC_DIR}/transport_adapter/transport_adapter_listener_impl.cc
    ${TM_SRC_DIR}/transport_adapter/transport_adapter_impl.cc
    ${TM_SRC_DIR}/transport_adapter/threaded_socket_connection.cc
   )
  set (src_tcp
    ${TM_SRC_DIR}/tcp/tcp_transport_adapter.cc
    ${TM_SRC_DIR}/tcp/tcp_client_listener.cc
    ${TM_SRC_DIR}/tcp/tcp_device.cc
    ${TM_SRC_DIR}/tcp/tcp_socket_connection.cc
    ${TM_SRC_DIR}/tcp/tcp_connection_factory.cc
   )

    source_group("Header Files" FILES ${inc} )
    source_group("Header Files\\Transport Manager" FILES ${inc_tm_adpt} )
    source_group("Header Files\\TCP" FILES ${inc_tcp} )
    source_group("Source Files" FILES ${src})
    source_group("Source Files\\Transport Manager" FILES ${src_tm_adpt})
    source_group("Source Files\\TCP" FILES ${src_tcp})

  if (BUILD_BT_SUPPORT)
    file (GLOB bt_inc ${TM_INC_DIR}/bluetooth/*.h )
    set( bt_src ${TM_SRC_DIR}/bluetooth/bluetooth_connection_factory.cc
                ${TM_SRC_DIR}/bluetooth/bluetooth_device_win.cc
                ${TM_SRC_DIR}/bluetooth/bluetooth_device_scanner_win.cc
                ${TM_SRC_DIR}/bluetooth/bluetooth_socket_connection_win.cc
                ${TM_SRC_DIR}/bluetooth/bluetooth_transport_adapter_win.cc)
    source_group("Header Files\\Bluetooth" FILES ${bt_inc} )
    source_group("Source Files\\Bluetooth" FILES ${bt_src} )
  endif()

  if (BUILD_USB_SUPPORT)
    set(usb_src ${TM_SRC_DIR}/usb/usb_aoa_adapter.cc
                ${TM_SRC_DIR}/usb/usb_connection_factory.cc
                ${TM_SRC_DIR}/usb/usb_device_scanner.cc)

    set (usb_inc ${TM_INC_DIR}/usb/common.h
                 ${TM_INC_DIR}/usb/usb_aoa_adapter.h
                 ${TM_INC_DIR}/usb/usb_connection_factory.h
                 ${TM_INC_DIR}/usb/usb_control_transfer.h
                 ${TM_INC_DIR}/usb/usb_device.h
                 ${TM_INC_DIR}/usb/usb_device_scanner.h)

    set(usb_libsrc ${TM_SRC_DIR}/usb/libusb/platform_usb_device.cc
                   ${TM_SRC_DIR}/usb/libusb/usb_connection.cc
                   ${TM_SRC_DIR}/usb/libusb/usb_handler.cc)

    set(usb_libinc ${TM_INC_DIR}/usb/libusb/platform_usb_device.h
                   ${TM_INC_DIR}/usb/libusb/usb_connection.h
                   ${TM_INC_DIR}/usb/libusb/usb_handler.h)
    source_group("Header Files\\USB" FILES ${usb_inc} )
    source_group("Source Files\\USB" FILES ${usb_src} )
    source_group("Header Files\\USB\\LibUSB" FILES ${usb_libinc} )
    source_group("Source Files\\USB\\LibUSB" FILES ${usb_libsrc} )
  endif(BUILD_USB_SUPPORT)

    add_library("TransportManager" ${inc} ${src}
                                   ${inc_tm_adpt} ${src_tm_adpt}
                                   ${inc_tcp}   ${src_tcp}
                                   ${bt_inc} ${bt_src}
                                   ${usb_inc} ${usb_src}
                                   ${usb_libinc}  ${usb_libsrc}
    )

    target_link_libraries("TransportManager" ProtocolLibrary )
else()
    if (BUILD_BT_SUPPORT)
      include_directories(
      )
    endif()

set (SOURCES
  ${TM_SRC_DIR}/transport_manager_impl.cc
  ${TM_SRC_DIR}/transport_manager_default.cc
  ${TM_SRC_DIR}/transport_adapter/transport_adapter_listener_impl.cc
  ${TM_SRC_DIR}/transport_adapter/transport_adapter_impl.cc
  ${TM_SRC_DIR}/transport_adapter/threaded_socket_connection.cc
  ${TM_SRC_DIR}/tcp/tcp_transport_adapter.cc
  ${TM_SRC_DIR}/tcp/tcp_client_listener.cc
  ${TM_SRC_DIR}/tcp/tcp_device.cc
  ${TM_SRC_DIR}/tcp/tcp_socket_connection.cc
  ${TM_SRC_DIR}/tcp/tcp_connection_factory.cc
)

if (BUILD_BT_SUPPORT)
  list (APPEND SOURCES
  ${TM_SRC_DIR}/bluetooth/bluetooth_device_scanner_posix.cc
  ${TM_SRC_DIR}/bluetooth/bluetooth_transport_adapter_posix.cc
  ${TM_SRC_DIR}/bluetooth/bluetooth_connection_factory.cc
  ${TM_SRC_DIR}/bluetooth/bluetooth_socket_connection_posix.cc
  ${TM_SRC_DIR}/bluetooth/bluetooth_device_posix.cc
  )
endif()

if (BUILD_USB_SUPPORT)
  list (APPEND SOURCES
    ${TM_SRC_DIR}/usb/usb_aoa_adapter.cc
    ${TM_SRC_DIR}/usb/usb_connection_factory.cc
    ${TM_SRC_DIR}/usb/usb_device_scanner.cc
  )
  if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list (APPEND SOURCES
    ${TM_SRC_DIR}/usb/libusb/usb_handler.cc
    ${TM_SRC_DIR}/usb/libusb/usb_connection.cc
    ${TM_SRC_DIR}/usb/libusb/platform_usb_device.cc
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "QNX")
     list(APPEND SOURCES
     ${TM_SRC_DIR}/usb/qnx/usb_handler.cc
     ${TM_SRC_DIR}/usb/qnx/usb_connection.cc
     ${TM_SRC_DIR}/usb/qnx/platform_usb_device.cc
     )
  endif()
endif(BUILD_USB_SUPPORT)


if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
endif()

add_library(${target} ${SOURCES})
target_link_libraries(${target} ProtocolLibrary)

if (BUILD_USB_SUPPORT)
  if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
    target_link_libraries(${target} usbdi)
  endif()
endif(BUILD_USB_SUPPORT)

if(ENABLE_LOG)
  target_link_libraries(${target} log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

if (BUILD_BT_SUPPORT)
 if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries(${target} bluetooth)
 endif()
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
    add_library("transport_manager" ${SOURCES}
            ${TRANSPORT_MANAGER_SOURCES}
    )
endif()
endif()

if(BUILD_TESTS)
  #add_subdirectory(test)
endif()
