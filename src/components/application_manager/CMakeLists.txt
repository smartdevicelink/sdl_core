include_directories (
  ./include/
  ../utils/include/
  ../JSONHandler/include/
  ../protocol_handler/include/
  ../SmartObjects/include/
  ${CMAKE_BINARY_DIR}/src/components/Interfaces
)

set (SOURCES
./src/message.cc
./src/api_version_converter.cc
./src/api_version_converter_v1.cc
./src/application_impl.cc
./src/basic_command_factory.cc
./src/command_impl.cc
./src/get_device_list_command.cc
./src/get_device_list_response_command.cc
)


add_library("ApplicationManager"
  ${SOURCES}
)
