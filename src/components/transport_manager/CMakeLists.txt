include_directories (
  ./include
  ../utils/include/
  ../protocol_handler/include
  ../connection_handler/include
  ../config_profile/include
  ../resumption/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/libusbx-1.0.16
)
if (BUILD_BT_SUPPORT)
  include_directories(
  )
endif()


set (SOURCES
  ./src/transport_manager_impl.cc
  ./src/transport_manager_default.cc
  ./src/transport_adapter/transport_adapter_listener_impl.cc
  ./src/transport_adapter/transport_adapter_impl.cc
  ./src/tcp/tcp_transport_adapter.cc
  ./src/transport_adapter/threaded_socket_connection.cc
  ./src/tcp/tcp_client_listener.cc
  ./src/tcp/tcp_device.cc
  ./src/transport_adapter/transport_adapter_event.cc
  ./src/tcp/tcp_socket_connection.cc
  ./src/tcp/tcp_connection_factory.cc
)

if (BUILD_AVAHI_SUPPORT)
  list (APPEND SOURCES
  ./src/tcp/dnssd_service_browser.cc
  )
endif()
if (BUILD_BT_SUPPORT)
  list (APPEND SOURCES
  ./src/bluetooth/bluetooth_device_scanner.cc
  ./src/bluetooth/bluetooth_transport_adapter.cc
  ./src/bluetooth/bluetooth_connection_factory.cc
  ./src/bluetooth/bluetooth_socket_connection.cc
  ./src/bluetooth/bluetooth_device.cc
  )
endif()

if (BUILD_USB_SUPPORT)
  list (APPEND SOURCES
    ./src/usb/usb_aoa_adapter.cc
    ./src/usb/usb_connection_factory.cc
    ./src/usb/usb_device_scanner.cc
  )
  if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list (APPEND SOURCES
    ./src/usb/libusb/usb_handler.cc
    ./src/usb/libusb/usb_connection.cc
    ./src/usb/libusb/platform_usb_device.cc
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    list(APPEND SOURCES
    ./src/usb/qnx/usb_handler.cc
    ./src/usb/qnx/usb_connection.cc
    ./src/usb/qnx/platform_usb_device.cc
    )
  endif()
endif(BUILD_USB_SUPPORT)

if (BUILD_MME_SUPPORT)
  list (APPEND SOURCES
    src/mme/mme_transport_adapter.cc
    src/mme/mme_device.cc
    src/mme/mme_device_scanner.cc
    src/mme/mme_connection_factory.cc
    src/mme/iap_connection.cc
    src/mme/iap2_connection.cc
  )
  if (MME_MQ)
    add_definitions(-DMME_MQ)
  endif(MME_MQ)
endif(BUILD_MME_SUPPORT)

add_library("TransportManager" ${SOURCES})

if (BUILD_USB_SUPPORT)
  if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
    target_link_libraries ("TransportManager" usbdi)
  endif()
endif(BUILD_USB_SUPPORT)

if (BUILD_MME_SUPPORT)
  target_link_libraries ("TransportManager" ipod)
  target_link_libraries ("TransportManager" iap2client)
  target_link_libraries ("TransportManager" mme)
  target_link_libraries ("TransportManager" qdb)
  if (MME_MQ)
    target_link_libraries("TransportManager" mq)
  endif(MME_MQ)
endif(BUILD_MME_SUPPORT)

if (BUILD_AVAHI_SUPPORT)
  target_link_libraries ("TransportManager" avahi-common)
endif()
target_link_libraries ("TransportManager" log4cxx)
if (BUILD_BT_SUPPORT)
  target_link_libraries("TransportManager" bluetooth)
endif()
