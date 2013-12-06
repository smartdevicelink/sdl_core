include_directories (
  ./include/
  ../application_manager/include/
  ../utils/include/
  ../config_profile/include/
  ../media_manager/include/
  ../protocol_handler/include
  ../../thirdPartyLibs/jsoncpp/include/
  ../../thirdPartyLibs/MessageBroker/include
)

set (SOURCES
./src/hmi_message_handler_impl.cc
./src/messagebroker_adapter.cc
./src/hmi_message_adapter.cc
)


add_library("HMIMessageHandler" ${SOURCES} )
target_link_libraries ("HMIMessageHandler" log4cxx)
