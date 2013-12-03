include_directories (
  ./include
  ../utils/include/
  ../protocol_handler/include
  ../connection_handler/include
)
if (BUILD_BT_SUPPORT)
  include_directories(
  ../../thirdPartyLibs/libusbx-1.0.16
  )
endif()

set (SOURCES
  ./src/transport_manager_impl.cc
  ./src/transport_manager_default.cc
  ./src/transport_adapter/transport_adapter_listener_impl.cc
  ./src/transport_adapter/transport_adapter_impl.cc
  ./src/timer.cc
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
  ./src/usb/libusb_handler.cc
  ./src/usb/usb_aoa_adapter.cc
  ./src/usb/usb_connection_factory.cc
  ./src/usb/usb_connection.cc
  ./src/usb/usb_device_scanner.cc
  )
endif()

add_library("TransportManager" ${SOURCES})
if (BUILD_AVAHI_SUPPORT)
  target_link_libraries ("TransportManager" avahi-common)
endif()
target_link_libraries ("TransportManager" log4cxx)
if (BUILD_BT_SUPPORT)
  target_link_libraries("TransportManager" bluetooth)
endif()

# vim: set ts=2 sw=2 et:
