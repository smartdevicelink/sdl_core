include_directories (
  ./include/
  ../utils/include/
  ../JSONHandler/include/
  ../protocol_handler/include/
  ../SmartObjects/include/
  ../hmi_message_handler/include
  ../mobile_message_handler/include
  ../ConnectionHandler/include/
  ../request_watchdog/include/
  ../TransportManager/include/
  ${CMAKE_BINARY_DIR}/src/components/Interfaces
)

set (SOURCES
./src/application_manager_impl.cc
./src/message.cc
./src/application_impl.cc
./src/basic_command_factory.cc
./src/commands/command_impl.cc
./src/commands/get_device_list_command.cc
./src/commands/get_device_list_response_command.cc
./src/commands/register_app_interface_command.cc
)


add_library("ApplicationManager" ${SOURCES} )
target_link_libraries ("ApplicationManager" log4cxx)
