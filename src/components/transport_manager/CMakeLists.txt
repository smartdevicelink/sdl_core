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

project(transport_manager)

add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src/transport_manager_default.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/transport_manager_impl.cc"
)

target_sources(${PROJECT_NAME}
  PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src/tcp/tcp_client_listener.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/tcp/tcp_connection_factory.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/tcp/tcp_device.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/tcp/tcp_server_originated_socket_connection.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/tcp/tcp_socket_connection.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/tcp/tcp_transport_adapter.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/tcp/network_interface_listener_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/transport_adapter/threaded_socket_connection.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/transport_adapter/transport_adapter_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/transport_adapter/transport_adapter_listener_impl.cc"
)

if(QNXNTO)
  target_sources(${PROJECT_NAME}
    PRIVATE
     "${CMAKE_CURRENT_LIST_DIR}/src/tcp/platform_specific/qnx/platform_specific_network_interface_listener.cc"
  )
else ()
  target_sources(${PROJECT_NAME}
    PRIVATE
     "${CMAKE_CURRENT_LIST_DIR}/src/tcp/platform_specific/linux/platform_specific_network_interface_listener.cc"
  )
endif()

if(BUILD_USB_SUPPORT)
  target_sources(${PROJECT_NAME}
    PRIVATE
      "${CMAKE_CURRENT_LIST_DIR}/src/usb/usb_aoa_adapter.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/usb/usb_connection_factory.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/usb/usb_device_scanner.cc"
  )
  target_compile_definitions(${PROJECT_NAME}
    PUBLIC
      USB_SUPPORT
  )

  if(QNXNTO)
    target_sources(${PROJECT_NAME}
      PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/usb/qnx/platform_usb_device.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/usb/qnx/usb_connection.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/usb/qnx/usb_handler.cc"
    )
    target_link_libraries(${PROJECT_NAME}
      PUBLIC
        usbdi
    )
  else()
    target_sources(${PROJECT_NAME}
      PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/usb/libusb/platform_usb_device.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/usb/libusb/usb_connection.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/usb/libusb/usb_handler.cc"
    )
  endif()
endif()

if(BUILD_BT_SUPPORT)
  if(QNXNTO)
    target_sources(${PROJECT_NAME}
      PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/bluetooth/bluetooth_connection_factory.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/bluetooth/bluetooth_connection.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/bluetooth/bluetooth_device.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/bluetooth/message_queue.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/bluetooth/bluetooth_listener.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/bluetooth/bluetooth_transport_adapter.cc"
    )
    target_compile_definitions(${PROJECT_NAME}
      PUBLIC
        BLUETOOTH_SUPPORT
    )
  endif()
endif()

if(BUILD_MME_SUPPORT)
  if(QNXNTO)
    SDL_find_package(ipod REQUIRED)
    SDL_find_package(iap2 REQUIRED)
    target_sources(${PROJECT_NAME}
      PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/mme_transport_adapter.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/mme_device.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/iap_device.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/iap2_device.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/mme_client_listener.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/mme_connection_factory.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/iap_connection.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/iap2_connection.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/protocol_config.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/protocol_connection_timer.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/protocol_manager.cc"
        "${CMAKE_CURRENT_LIST_DIR}/src/mme/protocol_descriptor.cc"
    )
    target_compile_definitions(${PROJECT_NAME}
      PUBLIC
        MME_SUPPORT
    )
    target_link_libraries(${PROJECT_NAME}
      PUBLIC
        ipod::ipod
        iap2::iap2
        qdb
    )
  endif()
endif()

if(BUILD_AOA_SUPPORT)
  SDL_find_package(aoa REQUIRED)
  target_sources(${PROJECT_NAME}
    PRIVATE
      "${CMAKE_CURRENT_LIST_DIR}/src/aoa/aoa_connection_factory.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/aoa/aoa_connection.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/aoa/aoa_device.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/aoa/aoa_dynamic_device.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/aoa/aoa_transport_adapter.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/aoa/aoa_wrapper.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/aoa/pps_listener.cc"
  )
  target_compile_definitions(${PROJECT_NAME}
    PUBLIC
      AOA_SUPPORT
  )
  target_link_libraries(${PROJECT_NAME}
    PUBLIC
      aoa::aoa
      pps
  )
endif()

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    utils::utils
    protocol::protocol
    config_profile::config_profile
    resumption::resumption
    $<$<BOOL:${SYNC_USE_MQ_SERVER}>:mq>
  PRIVATE
    components::interface
)

if(TELEMETRY_MONITOR)
  target_link_libraries(${PROJECT_NAME}
    PUBLIC
      telemetry_monitor::telemetry_monitor
  )
endif()

target_include_directories(${PROJECT_NAME}
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/transport_manager/tcp/platform_specific/qnx"
)

#if BUILD_TESTS (Not just yet)
