include_directories (
  ./include
  ../utils/include/
  ../protocol_handler/include
  ../connection_handler/include
)

set (SOURCES
	./src/transport_manager_impl.cc
	./src/transport_manager_default.cc
	./src/transport_adapter/transport_adapter_listener_impl.cc
	./src/transport_adapter/transport_adapter_impl.cc
	./src/timer.cc
	./src/bluetooth/bluetooth_adapter.cc
	./src/tcp/tcp_adapter.cc
	./src/transport_adapter/threaded_socket_connection.cc
	./src/tcp/tcp_client_listener.cc
	./src/tcp/tcp_device.cc
	./src/bluetooth/bluetooth_device_scanner.cc
	./src/transport_adapter/transport_adapter_event.cc
	./src/bluetooth/bluetooth_connection_factory.cc
	./src/bluetooth/bluetooth_socket_connection.cc
	./src/tcp/tcp_socket_connection.cc
	./src/bluetooth/bluetooth_device.cc
	./src/tcp/dnssd_service_browser.cc
	./src/tcp/tcp_connection_factory.cc
	./src/usb/libusb_handler.cc
	./src/usb/usb_aoa_adapter.cc
	./src/usb/usb_connection_factory.cc
	./src/usb/usb_connection.cc
	./src/usb/usb_device_scanner.cc
)

add_library("TransportManager" ${SOURCES})
target_link_libraries ("TransportManager" log4cxx bluetooth avahi-common)
