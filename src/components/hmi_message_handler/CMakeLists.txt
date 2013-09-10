include_directories (
  ./include/
  ../application_manager/include/
  ../utils/include/
  ../smart_objects/include/
  ../formatters/include/
  ../config_profile/include/
  ../../thirdPartyLibs/jsoncpp/include/
  ../../thirdPartyLibs/MessageBroker/include
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_BINARY_DIR}/src/components/dbus_schema/include/
  ${CMAKE_BINARY_DIR}/
  /usr/include/dbus-1.0/
  /usr/lib/i386-linux-gnu/dbus-1.0/include/
)

set (SOURCES
./src/hmi_message_handler_impl.cc
./src/messagebroker_adapter.cc
./src/hmi_message_adapter.cc
./src/to_hmi_thread_impl.cc
./src/from_hmi_thread_impl.cc
./src/dbus_message_adapter.cc
./src/dbus_message_controller.cc
./src/dbus_adapter.cc
)

add_library("HMIMessageHandler" ${SOURCES})
target_link_libraries ("HMIMessageHandler" DBusSchema log4cxx dbus-1)
