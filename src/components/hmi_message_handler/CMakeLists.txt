include_directories (
  ./include/
  ../application_manager/include/
  ../Utils/include/
  ../../thirdPartyLibs/jsoncpp/include/
  ../../thirdPartyLibs/MessageBroker/include
  ${CMAKE_BINARY_DIR}/src/components/Interfaces/
)

set (SOURCES
./src/hmi_message_handler_impl.cc
./src/messagebroker_adapter.cc
./src/hmi_message_adapter.cc
)


add_library("HMIMessageHandler"
  ${SOURCES}
)