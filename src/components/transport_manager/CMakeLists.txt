include_directories (
  ./include
  ../utils/include/
)

set (SOURCES
	./src/transport_manager_impl.cc
	./src/device_adapter_listener_impl.cc
	./src/device_adapter_impl.cc
)

add_library("TransportManager" ${SOURCES})
target_link_libraries ("TransportManager" log4cxx)
